#pragma once
#include "constants.hpp"
#include <stdexcept>
#include <memory>
#include <list>










class Order {
public:
  Order(OrderId orderId, Price price, Quantity quantity, Side side, OrderType ordertype);

  
  Order(OrderId orderId, Quantity quantity, Side side); //MarketOrder case
  
      OrderId GetOrderId() const   {return orderId_;}
      Price GetPrice() const { return price_; }
      Side GetSide() const { return side_; }
  Quantity GetInitialQuantity() const {return initialQuantity_;}
  Quantity GetRemainingQuantity() const { return remainingQuantity_; }
  OrderType GetOrderType() const {return orderType_;}
  bool IsFilled() const {return remainingQuantity_ == 0;}
  void Fill(const Quantity quantity) {
    if (quantity > remainingQuantity_) {
      throw std::logic_error("fill quantity > remaining quantity");
     
    }
    remainingQuantity_ -= quantity;
  }
  void ToGoodTillCancel(Price setPrice) {
    // set MARKET order price to worst price asked for!
    // enables matching as the orderbook is initially in non-matched state
    if (GetOrderType() != OrderType::MarketOrder) {
      throw std::logic_error("Conversion supported only from GoodTillCancel");
    }
    price_ = setPrice;
    orderType_ = OrderType::GoodTillCancel;
    }    
  
private:
  OrderId orderId_;
  Price price_;
  Quantity initialQuantity_;
  Quantity remainingQuantity_;
  Side side_;
  OrderType orderType_;  
  //add agent id for self match prevention
};


using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;
// orderpointers for a fixed given price
// can use vector also


// for add -> direct new order obj suffices
// for cancel -> orderId sufficent
// for modif -> new abstraction to implement cancel + add, via this type to indicate modif.

//for trade confirm -> trade object with buy and sell side info

class OrderModify {
public:
  OrderModify(OrderId orderId, Side side, Price price, Quantity quantity);
      OrderId GetOrderId() const   {return orderId_;}
      Price GetPrice() const { return price_; }
      Side GetSide() const { return side_; }
      Quantity GetQuantity() const { return quantity_; }


      
  OrderPointer ToOrderPointer(OrderType type) const;

    private:
      OrderId orderId_;
      Price price_;
      Side side_;
      Quantity quantity_;
};



