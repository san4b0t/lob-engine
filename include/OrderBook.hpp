#pragma once
#include "Types.hpp"
#include <map>
#include <list>
#include <unordered_map>
#include <vector>

class OrderBook
{
public:
    OrderBook() = default;

    std::vector<Trade> addOrder(Order order);
    bool cancelOrder(uint64_t orderId);

    uint64_t getBestBid() const;
    uint64_t getBestAsk() const;

private:
    using OrderList = std::list<Order>;
    using OrderIterator = OrderList::iterator;

    std::map<uint64_t, OrderList, std::greater<uint64_t>> bids_;
    std::map<uint64_t, OrderList, std::less<uint64_t>> asks_;

    std::unordered_map<uint64_t, OrderIterator> orderMap_;

    std::vector<Trade> matchOrder(Order &order);
};