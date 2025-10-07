#pragma once
#include "constants.hpp"
#include <memory>
#include <list>
#include <stdexcept>











class Order {
public:
  Order(OrderId orderId, Price price, Quantity quantity, Side side, OrderType ordertype) : orderId_(orderId), price_(price) , initialQuantity_(quantity), remainingQuantity_(initialQuantity_), side_(side), orderType_(ordertype){}

  //TODO: find appropriate default Price replacement (Marketorder)  
  Order(OrderId orderId, Quantity quantity, Side side) : Order(orderId, Constants::InvalidPrice, quantity, side, OrderType::MarketOrder){}
  
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
  OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
      : orderId_(orderId), price_(price), side_(side), quantity_(quantity) {}
      OrderId GetOrderId() const   {return orderId_;}
      Price GetPrice() const { return price_; }
      Side GetSide() const { return side_; }
      Quantity GetQuantity() const { return quantity_; }


      
  OrderPointer ToOrderPointer(OrderType type) const {
    //works more like a cancel-replace, specify type and return the shared ptr to modified
    //need to delete old one tho
    return std::make_shared<Order>( GetOrderId(), GetPrice(), GetQuantity(),  GetSide(),type);
      }

    private:
      OrderId orderId_;
      Price price_;
      Side side_;
      Quantity quantity_;
};



