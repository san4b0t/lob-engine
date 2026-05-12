#pragma once
#include <cstdint>

enum class Side
{
    BUY,
    SELL
};
enum class OrderType
{
    LIMIT,
    MARKET
};

struct Order
{
    uint64_t id;
    Side side;
    OrderType type;
    uint64_t price;
    uint64_t quantity;
};

struct Trade
{
    uint64_t maker_order_id;
    uint64_t taker_order_id;
    uint64_t price;
    uint64_t quantity;
};