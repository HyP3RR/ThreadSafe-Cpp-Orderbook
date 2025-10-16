
#include <cstddef>
#include <atomic>
#include "order.hpp"
#include <new>







class OrderQueue {
public:
  OrderQueue(): queue_{}, in_{0}, out_{0}, cachedIn_{0} , cachedOut_{0} {}
  void Send_Order(ClientOrderPointer order);
  void ProcessOrders();

private:
  static_assert(std::atomic<std::size_t>::is_always_lock_free);
  static constexpr std::size_t MAX_OUTSTANDING_ORDERS = 2048;
  std::shared_ptr<ClientOrder> queue_[MAX_OUTSTANDING_ORDERS];
  alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> in_;
  alignas(std::hardware_destructive_interference_size) std::atomic<std::size_t> out_; // handle ping pong
  std::size_t cachedIn_;
  std::size_t cachedOut_;
};

