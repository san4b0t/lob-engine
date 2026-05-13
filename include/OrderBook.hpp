#pragma once
#include "Types.hpp"
#include <map>
#include <list>
#include <unordered_map>
#include <vector>

using namespace std;

class OrderBook
{
public:
    OrderBook() = default;

    vector<Trade> addOrder(Order order);
    bool cancelOrder(uint64_t orderId);

    uint64_t getBestBid() const;
    uint64_t getBestAsk() const;

private:
    using OrderList = list<Order>;
    using OrderIterator = OrderList::iterator;

    map<uint64_t, OrderList, greater<uint64_t>> bids_;
    map<uint64_t, OrderList, less<uint64_t>> asks_;

    unordered_map<uint64_t, OrderIterator> orderMap_;

    vector<Trade> matchOrder(Order &order);
};