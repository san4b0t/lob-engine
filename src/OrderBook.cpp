#include "../include/OrderBook.hpp"

std::vector<Trade> OrderBook::addOrder(Order order)
{
    if (order.quantity == 0)
        return {};

    std::vector<Trade> trades = matchOrder(order);

    if (order.quantity > 0 && order.type == OrderType::LIMIT)
    {
        OrderIterator it;
        if (order.side == Side::BUY)
        {
            bids_[order.price].push_back(order);
            it = std::prev(bids_[order.price].end());
        }
        else
        {
            asks_[order.price].push_back(order);
            it = std::prev(asks_[order.price].end());
        }
        orderMap_[order.id] = it;
    }

    return trades;
}

std::vector<Trade> OrderBook::matchOrder(Order &order)
{
    std::vector<Trade> trades;

    if (order.side == Side::BUY)
    {
        auto it = asks_.begin();
        while (it != asks_.end() && order.quantity > 0 &&
               (order.type == OrderType::MARKET || order.price >= it->first))
        {

            OrderList &level = it->second;
            auto orderIt = level.begin();

            while (orderIt != level.end() && order.quantity > 0)
            {
                uint64_t matchedQty = std::min(order.quantity, orderIt->quantity);
                trades.push_back({orderIt->id, order.id, it->first, matchedQty});

                order.quantity -= matchedQty;
                orderIt->quantity -= matchedQty;

                if (orderIt->quantity == 0)
                {
                    orderMap_.erase(orderIt->id);
                    orderIt = level.erase(orderIt);
                }
                else
                {
                    ++orderIt;
                }
            }

            if (level.empty())
            {
                it = asks_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    else
    {
        auto it = bids_.begin();
        while (it != bids_.end() && order.quantity > 0 &&
               (order.type == OrderType::MARKET || order.price <= it->first))
        {

            OrderList &level = it->second;
            auto orderIt = level.begin();

            while (orderIt != level.end() && order.quantity > 0)
            {
                uint64_t matchedQty = std::min(order.quantity, orderIt->quantity);
                trades.push_back({orderIt->id, order.id, it->first, matchedQty});

                order.quantity -= matchedQty;
                orderIt->quantity -= matchedQty;

                if (orderIt->quantity == 0)
                {
                    orderMap_.erase(orderIt->id);
                    orderIt = level.erase(orderIt);
                }
                else
                {
                    ++orderIt;
                }
            }

            if (level.empty())
            {
                it = bids_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    return trades;
}

bool OrderBook::cancelOrder(uint64_t orderId)
{
    auto mapIt = orderMap_.find(orderId);
    if (mapIt == orderMap_.end())
        return false;

    OrderIterator listIt = mapIt->second;
    uint64_t price = listIt->price;
    Side side = listIt->side;

    if (side == Side::BUY)
    {
        bids_[price].erase(listIt);
        if (bids_[price].empty())
            bids_.erase(price);
    }
    else
    {
        asks_[price].erase(listIt);
        if (asks_[price].empty())
            asks_.erase(price);
    }

    orderMap_.erase(mapIt);
    return true;
}

uint64_t OrderBook::getBestBid() const { return bids_.empty() ? 0 : bids_.begin()->first; }
uint64_t OrderBook::getBestAsk() const { return asks_.empty() ? 0 : asks_.begin()->first; }