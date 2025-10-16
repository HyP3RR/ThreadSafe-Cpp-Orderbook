#include "spsc.hpp"
#include "order.hpp"
#include "orderbook.hpp"
#include <memory>
#include <thread>


OrderBook myorderbook;

void OrderQueue::Send_Order(ClientOrderPointer order) {
  // send modifies in_ ptr, process modifies out_ ptr...
  // cached in, out so we acquire only on demand.
  //works only for spsc right now.
    while (cachedIn_ == cachedOut_ + MAX_OUTSTANDING_ORDERS) {
      // filled! acquire on demand
      cachedOut_ = out_.load(std::memory_order_acquire);
      std::this_thread::yield();
    }
    
    queue_[cachedIn_ % MAX_OUTSTANDING_ORDERS] = order; 
    cachedIn_++;
    in_.fetch_add(1, std::memory_order_release);
}

void OrderQueue::ProcessOrders() {
  while (cachedOut_ == cachedIn_) {
      cachedIn_ = in_.load(std::memory_order_acquire);
      std::this_thread::yield();
      }
 
      // process cachedout
      auto order = queue_[cachedOut_ % MAX_OUTSTANDING_ORDERS];

      myorderbook.ProcessClientOrder(order);
      cachedOut_++;
      out_.fetch_add(1,std::memory_order_release);  
}
