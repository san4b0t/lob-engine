#include "../include/OrderBook.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std::chrono;

// Helper to generate a massive array of random orders
std::vector<Order> generateRandomOrders(size_t numOrders)
{
    std::vector<Order> orders;
    orders.reserve(numOrders);

    std::mt19937_64 rng(1337); // Fixed seed for reproducible benchmarks
    std::uniform_int_distribution<uint64_t> sideDist(0, 1);
    std::uniform_int_distribution<uint64_t> priceDist(90, 110);
    std::uniform_int_distribution<uint64_t> qtyDist(1, 100);

    for (size_t i = 0; i < numOrders; ++i)
    {
        Side side = (sideDist(rng) == 0) ? Side::BUY : Side::SELL;
        uint64_t price = priceDist(rng);
        uint64_t quantity = qtyDist(rng);

        orders.push_back({i + 1, side, OrderType::LIMIT, price, quantity});
    }
    return orders;
}

int main()
{
    const size_t WARMUP_COUNT = 10'000;
    const size_t MEASURE_COUNT = 1'000'000;
    const size_t TOTAL_ORDERS = WARMUP_COUNT + MEASURE_COUNT;

    std::cout << "Generating " << TOTAL_ORDERS << " random orders..." << std::endl;
    std::vector<Order> orders = generateRandomOrders(TOTAL_ORDERS);

    OrderBook book;

    // --- PHASE 1: WARM-UP ---
    // Process the first batch to fill CPU caches and warm up branch predictors
    std::cout << "Warming up with " << WARMUP_COUNT << " orders..." << std::endl;
    for (size_t i = 0; i < WARMUP_COUNT; ++i)
    {
        book.addOrder(orders[i]);
    }

    // --- PHASE 2: MEASUREMENT ---
    std::cout << "Measuring latency for " << MEASURE_COUNT << " orders..." << std::endl;

    // Start High-Resolution Timer
    auto start_time = high_resolution_clock::now();

    for (size_t i = WARMUP_COUNT; i < TOTAL_ORDERS; ++i)
    {
        book.addOrder(orders[i]);
    }

    // Stop Timer
    auto end_time = high_resolution_clock::now();

    // --- PHASE 3: REPORTING ---
    auto total_duration_ns = duration_cast<nanoseconds>(end_time - start_time).count();

    double average_latency_ns = static_cast<double>(total_duration_ns) / MEASURE_COUNT;
    double ops_per_second = (static_cast<double>(MEASURE_COUNT) / total_duration_ns) * 1e9;

    std::cout << "\n=== Benchmark Results ===" << std::endl;
    std::cout << "Total Time:       " << total_duration_ns / 1'000'000.0 << " ms" << std::endl;
    std::cout << "Average Latency:  " << std::fixed << std::setprecision(2) << average_latency_ns << " ns/order" << std::endl;
    std::cout << "Throughput:       " << std::fixed << std::setprecision(0) << ops_per_second << " ops/sec" << std::endl;

    return 0;
}