#include "orderbook.hpp"



void add_order(OrderBook &ob, Price price, Quantity quantity, Side side);

void print_orderbook(OrderBook &ob);


inline Side buy = Side::Buy;
inline Side sell = Side::Sell;
