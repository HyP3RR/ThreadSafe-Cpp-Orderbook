
#include "constants.hpp"
#include "order.hpp"
#include "orderbook.hpp"
#include <cstddef>
#include <iomanip>
#include <memory>
#include <iostream>
#include <ostream>


OrderBook orderbook;
OrderType type;

void print_Trade(Trades trades) {
  for (auto &trade : trades) {
    trade.PrintTrade();
    }
}


void add_order(Price price, Quantity quantity, Side side) {
  //client thread adding to spsc queue
  static int i = 0;
  OrderPointer ptr = std::make_shared<Order>(i++, price, quantity, side,
                                             type);
  auto trades = orderbook.AddOrder(ptr);
  print_Trade(trades);
}


void add_market(Quantity quantity, Side side) {
  static int i = 0;
  OrderPointer ptr = std::make_shared<Order>(i++,  quantity, side);
  auto trades = orderbook.AddOrder(ptr);
}

void cancel_order(OrderId orderId) {
  orderbook.CancelOrder(orderId);  
}

void modify_order(OrderId orderId, Side side,Price new_price, Quantity new_quantity) {
  OrderModify order{orderId, side,new_price, new_quantity};
  return print_Trade(orderbook.ModifyOrder(order));
}


void print_orderbook() {
  auto book = orderbook.GetOrderInfos();
  auto &bids = book.GetBids();
  auto &asks = book.GetAsks();

  const auto la = 30; //left align number
  const auto ra = 20; //right align number
  
  std::cout << std::left;
  std::cout << std::setw(la) << "Buy Price (Quantity)" << std::setw(ra) << "Sell Price (Quantity)" << "\n";
  std::cout << std::string(la, '-') <<std::string(ra,'-') << "\n";

  std::size_t levels = book.GetSize();
  for (std::size_t i = 0; i < levels; i++) {
    std::cout << std::setw(la);
    std::ostringstream oss;
    if (i < bids.size()) oss  <<bids[i].price_ <<" ( " << bids[i].quantity_ <<" ) ";
    else oss <<"-";
    std::cout << oss.str();

    std::cout << std::setw(ra);
    oss.str("");oss.clear(); //clear 
    if (i < asks.size())
      oss << asks[i].price_ << " ( " << asks[i].quantity_ << " ) ";
    else oss <<"-";
    std::cout <<oss.str() <<"\n";
    }
  
  
}

Side buy = Side::Buy;
Side sell = Side::Sell;

int main() {
  type = OrderType::GoodTillCancel;  
  add_order(10, 10, buy);
  add_order(9, 10, buy);
  add_order(5, 10, buy);
  add_order(7, 10, buy);
  add_order(11, 10, sell);
  add_order(12, 10, sell);
  add_order(10, 10, sell);
  add_order(9, 5, sell);
  add_order(1, 100, sell);
  
 
  type = OrderType::FillAndKill;
  add_order(12, 84, buy);
  type = OrderType::GoodTillCancel;
  add_order(10, 10, buy);
  add_order(9, 10, buy);
  add_order(5, 10, buy);
  add_order(7, 10, buy);
  type = OrderType::FillAndKill;
  add_order(3, 29, sell);
  add_market(7,buy);
  // prints, 7 (1) , 5 (10) on buy
  // 12 (4) on sell!
  modify_order(5, sell, 100, 4);
  cancel_order(5);
  // prints, 7 (1) , 5 (1)  on buy
  // 12 (4) on sell!
   print_orderbook();
}
