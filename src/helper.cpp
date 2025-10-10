#include "helper.hpp"
#include <iomanip>
#include <iostream>

void add_order(OrderBook &ob, Price price, Quantity quantity, Side side) {
  static int i = 0;
  OrderPointer ptr = std::make_shared<Order>(i++, price, quantity, side,
                                             OrderType::GoodTillCancel);
  auto trades = ob.AddOrder(ptr);
}




void print_orderbook(OrderBook &ob) {
  auto book = ob.GetOrderInfos();
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

