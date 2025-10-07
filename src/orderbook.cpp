#include "order.hpp"
#include "trades.hpp"
#include <cstddef>
#include <map>
#include <stdexcept>

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
  OrderbookLevelInfos(const LevelInfos &bids, const LevelInfos &asks)
      : bids_(bids), asks_(asks) {}

  const LevelInfos& GetBids() { return bids_; }
  const LevelInfos& GetAsks() {return asks_;}
private:
  LevelInfos bids_;
  LevelInfos asks_;
};


class OrderBook {
private:
  struct OrderEntry {
    OrderPointer order_{nullptr};
    OrderPointers::iterator location_;
    };

  std::map<Price, OrderPointers, std::greater<Price>> bids_;
  std::map<Price, OrderPointers, std::less<Price>> asks_;

  std::map<OrderId, OrderEntry> orders_;

  bool CanMatch(Side side, Price price) const {
    // if it can match
    if (side == Side::Sell) {
      if (bids_.empty())
        return false;
      auto &[bestBid, _] = *bids_.begin();
      return price <= bestBid;
    }
    else {
      if (asks_.empty())
        return false;
      auto &[bestAsk, _] = *asks_.begin();
      return price <= bestAsk;    
    }

    }
    Trades MatchOrder() {
      // loop and keep matching order. return each trade done
      Trades trades;
      trades.reserve(orders_.size());
      
      while (true) {
	 if (bids_.empty() || asks_.empty()) break;
        
       auto &[bidPrice, bidList] = *bids_.begin();
       auto &[askPrice, askList] = *asks_.begin();
       if (bidPrice < askPrice)
         break;

       while (bidList.size() && askList.size()) {
	 auto &currentBid = bidList.front();
       auto &currentAsk = askList.front();
       
       Quantity matchQuantity =
         std::min(currentBid->GetRemainingQuantity(), currentAsk->GetRemainingQuantity());
       currentBid->Fill(matchQuantity);
       currentAsk->Fill(matchQuantity);

       trades.emplace_back(TradeInfo{currentBid->GetOrderId(),
                                     currentBid->GetPrice(), matchQuantity},
                           TradeInfo{currentAsk->GetOrderId(),
                                     currentAsk->GetPrice(), matchQuantity});
       if (currentAsk->IsFilled()) {
         askList.pop_front();
         orders_.erase(currentAsk->GetOrderId());
       }
       
       if (currentBid->IsFilled()) {
         bidList.pop_front();
         orders_.erase(currentBid->GetOrderId());
         }
       }
       if (askList.empty())
         asks_.erase(asks_.begin());
       if (bidList.empty())
         bids_.erase(bids_.begin());
      }

      // cancel rest for FAK, guaranteed to be unique in the list (others
      // must've been matched due to the invariant).      
      if (asks_.empty()) {
        auto &[_, asks] = *asks_.begin();
        auto& order = asks.front();
        if(order->GetOrderType() == OrderType::FillAndKill) CancelOrder(order->GetOrderId());
        }
      if (bids_.empty()) {
        auto &[_, bids] = *bids_.begin();
        auto &order = bids.front();
        if(order->GetOrderType() == OrderType::FillAndKill) CancelOrder(order->GetOrderId());
        }
      
      return trades;
    }
    
public:
  Trades AddOrder(OrderPointer order) {
    
    if (orders_.count(order->GetOrderId()))
      return {}; // sanity check

    if (order->GetOrderType() == OrderType::FillAndKill &&
        !CanMatch(order->GetSide(), order->GetPrice()))
      return {};

    OrderPointers::iterator iterator;
    if (order->GetSide() == Side::Sell) {
      auto& orders = asks_[order->GetPrice()];
      orders.push_back(order);
      iterator = std::next(orders.begin(), orders.size()-1);
    } else {
      auto &orders = bids_[order->GetPrice()];
      orders.push_back(order);
      iterator = std::next(orders.begin(), orders.size() -1 );
    }
    
     orders_.insert({order->GetOrderId() , OrderEntry{order, iterator}});
    return MatchOrder();     
  }

  void CancelOrder(OrderId orderId) {}

  Trades ModifyOrder(OrderModify order) {
    //cancel + add logic, by converting OrderModify to pointer etc.
    }
    std::size_t Size() const { // size of orders_
    }
    OrderbookLevelInfos GetOrderInfos() const {
      // snapshot of orderbook returned, create and send.
      // either create on the fly here and send
      // or maintain the LevelInfo and make both synchronous
// can make it on the fly in a vector to keep it simple.       

      
      }
      
};
