#pragma once
#include <cstdint>
#include <limits>

//support for add, modify and cancel too

enum class OrderType {
  GoodTillCancel,
  FillAndKill,
  MarketOrder
  };

enum class Side {
  Buy,
  Sell
};
using Quantity = std::uint32_t;
using Price = std::int32_t;
using OrderId = std::uint64_t;


struct Constants {
  static const Price InvalidPrice = std::numeric_limits<Price>::quiet_NaN(); 
};
