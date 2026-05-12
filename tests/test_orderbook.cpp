#include <gtest/gtest.h>
#include "../include/OrderBook.hpp"

TEST(OrderBookTest, AddAndCancelOrder)
{
    OrderBook book;
    book.addOrder({1, Side::BUY, OrderType::LIMIT, 100, 10});
    EXPECT_EQ(book.getBestBid(), 100);

    bool cancelled = book.cancelOrder(1);
    EXPECT_TRUE(cancelled);
    EXPECT_EQ(book.getBestBid(), 0); // Book is empty
}

TEST(OrderBookTest, CrossSpreadMatching)
{
    OrderBook book;
    book.addOrder({1, Side::SELL, OrderType::LIMIT, 100, 10});
    auto trades = book.addOrder({2, Side::BUY, OrderType::LIMIT, 105, 5});

    EXPECT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].price, 100);
    EXPECT_EQ(trades[0].quantity, 5);
    EXPECT_EQ(book.getBestAsk(), 100); // 5 left on the ask
}