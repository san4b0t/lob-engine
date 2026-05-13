# Limit Order Book (LOB) Matching Engine

![Build Status](https://github.com/san4b0t/lob-engine/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

A **high-performance, production-grade Limit Order Book matching engine** built in C++. Designed for **sub-microsecond latency** and suitable for **quantitative trading systems**, this project demonstrates core exchange mechanics with strict **price-time priority**, **O(1) order cancellation**, and memory-safe implementation.

---

## 🚀 Features

- **Price-Time Priority Matching**: Strict FIFO execution within each price level
- **Order Types**: Market orders, limit orders, cancellations, and modifications
- **O(1) Order Cancellation**: Achieved via hash map + doubly-linked list design
- **Ultra-Low Latency**: **96.62 ns/order** average latency, **10.35M ops/sec** throughput
- **Memory Safe**: Valgrind-verified, zero leaks, RAII-compliant C++17 code
- **Fully Tested**: Comprehensive Google Test suite with edge-case coverage
- **Production-Ready CI**: Automated builds, tests, and memory checks via GitHub Actions
- **Dockerized**: Reproducible builds and benchmarks in containerized environment

---

## 📊 Benchmark Performance

Measured on local hardware with **1,000,000 orders** after warm-up:

### Benchmark Results ###
Total Time: 96.617 ms
Average Latency: 96.62 ns/order
Throughput: 10,350,141 ops/sec

**Key Takeaways:**

- **Sub-100 nanosecond** order insertion/matching latency
- **Over 10 million operations per second**
- Measured in release mode with `-O3` optimizations
- Benchmark includes random limit/market orders across multiple price levels

---

## 🏗️ Architecture

### Core Data Structures

The engine uses a **three-layer architecture** optimized for speed:
```text
┌─────────────────────────────────────────────────────┐
│ OrderBook (per symbol)                              │
│                                                     │
│ ┌─────────────────────────────────────────────────┐ │
│ │ std::map<Price, PriceLevel>                     │ │
│ │ - Automatically sorted by price                 │ │
│ │ - O(log n) price level lookup                   │ │
│ └─────────────────────────────────────────────────┘ │
│                          ↓                          │
│ ┌─────────────────────────────────────────────────┐ │
│ │ PriceLevel                                      │ │
│ │ - std::list<Order> (doubly-linked)              │ │
│ │ - FIFO order queue                              │ │
│ │ - O(1) insertion at back                        │ │
│ └─────────────────────────────────────────────────┘ │
│                          ↓                          │
│ ┌─────────────────────────────────────────────────┐ │
│ │ std::unordered_map<OrderID, list::iterator>     │ │
│ │ - O(1) order lookup by ID                       │ │
│ │ - O(1) cancellation via iterator                │ │
│ └─────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────┘
```
### Why This Design?

| Component        | Data Structure                     | Purpose                                   | Complexity             |
| ---------------- | ---------------------------------- | ----------------------------------------- | ---------------------- |
| **Price Levels** | `std::map<Price, PriceLevel>`      | Maintain sorted prices for best bid/ask   | O(log n) insert/delete |
| **Order Queue**  | `std::list<Order>`                 | FIFO within price level, stable iterators | O(1) push_back         |
| **Order Lookup** | `std::unordered_map<ID, iterator>` | Fast cancellation without search          | O(1) lookup/erase      |

**Trade-off**: We accept O(log n) price insertion for automatic sorting and clean best bid/ask retrieval. In production, a custom tree or skip list could reduce this further.

---

## 🛠️ Build Instructions

### Prerequisites

- **C++17** compiler (GCC 7+, Clang 6+, or MSVC 2017+)
- **CMake** 3.14+
- **Google Test** (fetched automatically by CMake)
- **Docker** (optional, for containerized builds)

### Local Build

#### Clone the repository

```bash
git clone https://github.com/san4b0t/lob-engine.git
cd lob-engine
```

#### Configure with CMake

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

#### Build

```bash
cmake --build . -j$(nproc)
```

#### Run tests

```bash
ctest --output-on-failure
```

#### Run benchmarks

```bash
./benchmarks/run_benchmarks
```

### Debug Build with Sanitizers

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build .
./tests/order_book_tests
```

### Docker Build

#### Build Docker image

```bash
docker build -t lob-engine .
```

#### Run tests in container

```bash
docker run --rm lob-engine ctest --output-on-failure
```

#### Run benchmarks in container

```bash
docker run --rm lob-engine ./run_benchmarks
```

---

## 🧪 Testing

#### Run Unit Tests

```bash
cd build
./tests/order_book_tests
```

#### Memory Leak Check (Valgrind)

```bash
valgrind --leak-check=full --show-leak-kinds=all ./tests/order_book_tests
```

#### Test Coverage

- ✅ Basic order insertion and cancellation
- ✅ Price-time priority enforcement
- ✅ Limit order crossing spread
- ✅ Market order full/partial fills
- ✅ FIFO behavior within price levels
- ✅ Edge cases: empty book, invalid orders, duplicate IDs
- ✅ Stress tests: 10,000+ interleaved orders

---

## 📦 Project Structure

```text
.
├── src/
│ ├── Order.cpp
│ ├── PriceLevel.cpp
│ ├── OrderBook.cpp
│ └── MatchingEngine.cpp
├── include/
│ ├── Order.h
│ ├── PriceLevel.h
│ ├── OrderBook.h
│ └── MatchingEngine.h
├── tests/
│ └── order_book_tests.cpp
├── benchmarks/
│ └── latency_benchmark.cpp
├── docker/
│ └── Dockerfile
├── .github/
│ └── workflows/
│ └── ci.yml
├── CMakeLists.txt
└── README.md
```
---

## 🔄 CI/CD Pipeline

Every push triggers:

- ✅ **Build** (Debug + Release modes)
- ✅ **Unit Tests** (Google Test)
- ✅ **Memory Safety** (Valgrind leak checks)
- ✅ **Sanitizers** (AddressSanitizer in Debug builds)

See [`.github/workflows/ci.yml`](.github/workflows/ci.yml) for full configuration.

---

## 🎯 Use Cases

This matching engine is suitable for:

- **Quantitative trading system prototypes**
- **Exchange simulation and backtesting**
- **Low-latency order routing systems**

---

## 🚧 Future Enhancements

- [ ] **Iceberg Orders**: Hidden liquidity with display quantity
- [ ] **Stop Orders**: Conditional order activation
- [ ] **Multi-Symbol Support**: Concurrent order books with sharding
- [ ] **Event Sourcing**: Full audit trail and replay capability
- [ ] **Lock-Free Design**: Move to lock-free queues for multi-threading
- [ ] **Binary Protocol**: FIX/SBE protocol support for realistic input
- [ ] **Persistence Layer**: Crash recovery and snapshotting
- [ ] **Metrics Export**: Prometheus-style monitoring hooks

---

## 🤝 Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure CI passes
5. Submit a pull request

---

## 📄 License

MIT License. See [LICENSE](LICENSE) for details.

---

## 👤 Author

**Sanfo**

📧 [sanfo.bt@gmail.com](mailto:sanfo.bt@gmail.com)  
🔗 [LinkedIn](https://linkedin.com/in/sanfo-thomas)  
💻 [GitHub](https://github.com/san4b0t)

---

## 🙏 Acknowledgments

- Inspired by production exchange architectures at major trading venues
- Built with guidance from **low-latency systems design** best practices
- Uses **Google Test**, **CMake**, and **Docker** for modern C++ workflows

---

**⚡ Built for speed. Designed for production. Optimized for quant interviews.**
