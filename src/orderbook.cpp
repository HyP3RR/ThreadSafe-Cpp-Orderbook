#include "orderbook.hpp"
#include "order.hpp"
#include <algorithm>
#include <cstddef>


//Level Info methods
OrderbookLevelInfos::OrderbookLevelInfos(const LevelInfos &bids, const LevelInfos &asks)
      : bids_(bids), asks_(asks) {}


const LevelInfos& OrderbookLevelInfos::GetBids() { return bids_; }
const LevelInfos&  OrderbookLevelInfos::GetAsks() {return asks_; }

std::size_t OrderbookLevelInfos::GetSize() const {return std::max(bids_.size(),asks_.size());}


//Orderbook Methods

bool OrderBook::CanMatch(Side side, Price price) const {
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

Trades OrderBook::MatchOrder() {
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
      if (!asks_.empty()) {
        auto &[_, asks] = *asks_.begin();
        auto& order = asks.front();
        if(order->GetOrderType() == OrderType::FillAndKill) CancelOrder(order->GetOrderId());
        }
      if (!bids_.empty()) {
        auto &[_, bids] = *bids_.begin();
        auto &order = bids.front();
        if(order->GetOrderType() == OrderType::FillAndKill) CancelOrder(order->GetOrderId());
        }
  
      return trades;
}


//public API
Trades OrderBook::AddOrder(OrderPointer order) {
    
    if (orders_.count(order->GetOrderId()))
      return {}; // sanity check

    if (order->GetOrderType() == OrderType::FillAndKill &&
        !CanMatch(order->GetSide(), order->GetPrice()))
      return {};

    OrderPointers::iterator iterator;

    if (order->GetSide() == Side::Sell) {
      if(!asks_.count(order->GetPrice()))asks_.insert({order->GetPrice(),OrderPointers{}});
      auto& orders = asks_.at(order->GetPrice());
      orders.push_back(order);
      iterator = std::next(orders.begin(), orders.size()-1);
    }
      
    else {
      if(!bids_.count(order->GetPrice()))bids_.insert({order->GetPrice(),OrderPointers{}});
      auto &orders = bids_.at(order->GetPrice());
      orders.push_back(order);
      iterator = std::next(orders.begin(), orders.size() -1 );
    }
   
    orders_.insert({order->GetOrderId() , OrderEntry{order, iterator}});
    return MatchOrder();
}


void OrderBook::CancelOrder(OrderId orderId) {
    if (!orders_.count(orderId))
      return;

    const auto &[order, orderIterator] = orders_[orderId];
    auto price = order->GetPrice();
    if (order->GetSide() == Side::Sell) {
      auto &list = asks_.at(price);
      list.erase(orderIterator);
      if(list.empty()) asks_.erase(price);
    } else {
      auto &list = bids_.at(price);
        list.erase(orderIterator );
	if(list.empty()) bids_.erase(price);
    }

    //note, no cleanup for orders_ for now.
}

Trades OrderBook::ModifyOrder(OrderModify order) {
    // cancel + add logic, by converting OrderModify to pointer etc.    
    if (!orders_.count(order.GetOrderId()))
      return {};
    const auto &[existingOrder, _] = orders_.at(order.GetOrderId());
    CancelOrder(order.GetOrderId()); //only removes from orderbook
    return AddOrder(order.ToOrderPointer(existingOrder->GetOrderType()));  
}

std::size_t OrderBook::Size() const {
   // size of orders_
   return orders_.size();
}

OrderbookLevelInfos OrderBook::GetOrderInfos() const {
 // create and return snapshot of all levels of a orderbook
      // can return K levels of orderbook to keep it simple.
      
      LevelInfos asksSnapshot;
      LevelInfos bidsSnapshot;
      asksSnapshot.reserve(this->Size());
      bidsSnapshot.reserve(this->Size());

      auto CreateLevelInfo = [](Price price, const OrderPointers &orders) {
        Quantity levelQuantity = 0;
        for (auto &order : orders) {
          levelQuantity += order->GetRemainingQuantity();
        }
        return LevelInfo{price, levelQuantity, orders.size()};
      };
      
      for (auto &[price, orders] : asks_) {
        asksSnapshot.emplace_back(CreateLevelInfo(price, orders));
      }
      for (auto &[price, orders] : bids_) {
	bidsSnapshot.emplace_back(CreateLevelInfo(price, orders));
      }
      return OrderbookLevelInfos{bidsSnapshot, asksSnapshot};
}
