#pragma once
#include "constants.hpp"
#include <stdexcept>
#include <memory>
#include <list>
#include <future>
#include "trades.hpp"








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



class ClientOrder {
public:
  ClientOrder() {}
  ClientOrder(OrderId orderid, OrderCategory category, OrderType type, Price price, Quantity quantity, Side side, std::promise<Trades>&& q) : orderId_(orderid), category_(category), type_(type), price_(price), quantity_(quantity), side_(side),returned_trades(std::move(q)) {}
  OrderId GetOrderId() const { return orderId_; }
  OrderCategory GetOrderCategory() const { return category_; }
  OrderType GetOrderType() const  { return type_; }
  Price GetPrice() const  { return price_; }
  Quantity GetQuantity()const { return quantity_; }
  Side GetSide() const {return side_;}
  void returnPromise(const Trades& trades) {returned_trades.set_value(trades);}
  OrderPointer ClientToNewOrder() {
    if (category_ != OrderCategory::NewOrder)
      throw std::logic_error("Cannot convert to new order");
    if (type_ == OrderType::MarketOrder) {
      return std::make_shared<Order>(orderId_,quantity_,side_);
    } else  {
      return std::make_shared<Order>(orderId_, price_, quantity_, side_, type_);
      }
  }
  
private:
  OrderId orderId_;
  OrderCategory category_;
  OrderType type_;
  Price price_;
  Quantity quantity_;
  Side side_;
  std::promise<Trades> returned_trades; //set trades inside this.
};

using ClientOrderPointer = std::shared_ptr<ClientOrder>;
