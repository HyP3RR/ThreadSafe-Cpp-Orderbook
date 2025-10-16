#pragma once
#include "constants.hpp"
#include "order.hpp"
#include "trades.hpp"
#include <algorithm>
#include <cstddef>
#include <map>

/*


  LevelInfo -> for snapshot stream, create on demand in orderbook member
function
 Orderbook -> Core Data structure with matching logic.
 no self match prevention.
 before and after each func call, the invariant of no pending matches is true.
 */

struct LevelInfo {
  Price price_;
  Quantity quantity_;
  std::size_t orderCount_;
};

using LevelInfos =
    std::vector<LevelInfo>; // create LevelInfos on demand in orderbook


class OrderbookLevelInfos {
public:
  OrderbookLevelInfos(const LevelInfos &bids, const LevelInfos &asks);
  std::size_t GetSize() const;
  const LevelInfos& GetBids();
  const LevelInfos& GetAsks();
private:
  LevelInfos bids_;
  LevelInfos asks_;
};




// orderbook

class OrderBook {
private:
  struct OrderEntry {
    OrderPointer order_{nullptr};
    OrderPointers::iterator location_;
    };

  std::map<Price, OrderPointers, std::greater<Price>> bids_;
  std::map<Price, OrderPointers, std::less<Price>> asks_;

  std::map<OrderId, OrderEntry> orders_;

  bool CanMatch(Side side, Price price) const;
  Trades MatchOrder();
  
public:
  Trades AddOrder(OrderPointer order);

  void CancelOrder(OrderId orderId);

  Trades ModifyOrder(OrderModify order);

  std::size_t Size() const; //total orders_
  void ProcessClientOrder(ClientOrderPointer order);
  OrderbookLevelInfos GetOrderInfos() const;
};


