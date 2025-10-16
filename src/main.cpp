#include "constants.hpp"
#include "helper.hpp" //helper for client
#include "order.hpp"
#include "orderbook.hpp"
#include "spsc.hpp" // send orders here
#include "trades.hpp"
#include <thread>

extern OrderBook myorderbook;
OrderQueue orderqueue; //shared data structure
std::atomic<int> processed = 0;
void clientThread(Price price, Quantity quantity, Side side) {
  static OrderId order_id = 0;
  std::promise<Trades> t;
  std::future<Trades> ret = t.get_future();
  ClientOrder order{
    order_id++, OrderCategory::NewOrder, OrderType::GoodTillCancel, price, quantity, side,std::move(t)};
  orderqueue.Send_Order(std::make_shared<ClientOrder>(std::move(order))); //calls copy ctor otherwise, to make data on heap
  Trades trades = ret.get();
  std::cout << "Trades occured are \n";
  for (auto &trade : trades) {
    trade.PrintTrade();
  }
  processed++;
}

void writerThread() {
  while (processed < 1) {
      orderqueue.ProcessOrders();
    }
}

int main() {
 std::thread t2(writerThread);
 clientThread(10, 10, Side::Buy);
 clientThread(900,1,Side::Sell);
  t2.join();
  print_orderbook(myorderbook);
}
