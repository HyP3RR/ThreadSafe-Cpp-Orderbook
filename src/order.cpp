#include "order.hpp"
#include "constants.hpp"
#include <memory>


// basic order
Order::Order(OrderId orderId, Price price, Quantity quantity, Side side,
             OrderType ordertype)
    : orderId_(orderId), price_(price), initialQuantity_(quantity),
      remainingQuantity_(initialQuantity_), side_(side), orderType_(ordertype) {
}

//marketorder case
Order::Order(OrderId orderId, Quantity quantity, Side side)
    : Order(orderId, Constants::InvalidPrice, quantity, side,
            OrderType::MarketOrder) {}




// Modifyorder API
OrderModify::OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
      : orderId_(orderId), price_(price), side_(side), quantity_(quantity) {}

OrderPointer OrderModify::ToOrderPointer(OrderType type) const{
    //works more like a cancel-replace, specify type and return the shared ptr to modified
    //need to delete old one tho
    return std::make_shared<Order>( GetOrderId(), GetPrice(), GetQuantity(),  GetSide(),type);
};
