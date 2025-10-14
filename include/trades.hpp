#pragma once
#include "constants.hpp"
#include <vector>
#include <iostream>

struct TradeInfo {
  OrderId orderId_;
  Price price_;
  Quantity quantity_;
};

class Trade {
public:
  
  Trade(const TradeInfo& bidTrade, const TradeInfo& askTrade) : bidTrade_(bidTrade) , askTrade_(askTrade) {}

  const TradeInfo &GetBidTrade() { return bidTrade_; }
  const TradeInfo &GetAskTrade() {return askTrade_;}

  void PrintTrade() {
    std::cout << "Trade: \n";
    std::cout << "Buy: OrderID " << bidTrade_.orderId_
              << " Price: " << bidTrade_.price_
              << "  Qty: " << bidTrade_.quantity_ << "\n";
    std::cout <<"Sell: OrderId " <<askTrade_.orderId_ <<" Price: " <<askTrade_.price_ <<" Qty: " <<askTrade_.quantity_ <<"\n"; 
    }
  
private:
  TradeInfo bidTrade_;
  TradeInfo askTrade_;
  
};

using Trades = std::vector<Trade>;
