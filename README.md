# Thread-Safe C++ Orderbook

An ongoing implementation of a thread-safe orderbook in C++, inspired by learnings from my Alphagrep internship.  
The project focuses on core trading system mechanics such as order matching, execution handling, and safe concurrent access.

---

## 🚀 Features
- **Order Types**: Supports GTC (Good Till Cancel), FAK (Fill And Kill), and Market Orders.
- **Matching Engine**: Implements price–time priority with support for partial fills and immediate execution.
- **Thread Safety**: Dedicated orderbook writer thread with thread-safe queues ensures reliable and consistent request/response handling.
- **Execution Handling**: Robust handling of add, cancel, and modify operations.

---
## Directory Structure
```bash
src/
├── constants.hpp #constants and typenames for orderbook data
├── main.cpp # Entry point for the application
├── orderbook.cpp # Core orderbook logic
├── orderbook.hpp # Header file for orderbook
├── order.cpp # order API helper implementatioon
├── order.hpp # Header file for order API
├── trades.cpp # Trade info implementation
└── trades.hpp # Header file for trades representation
```

---
## ⚙️ Architecture
- **Orderbook Core**: Maintains bid/ask levels with efficient lookup and matching.
- **Writer Thread**: Processes incoming order requests sequentially, avoiding race conditions.
- **Thread-Safe Queues**: Used for passing requests (add/cancel/modify) and receiving responses.

---

## 🛠️ Tech Stack
- **Language**: C++17 (or later)
- **Concurrency**: `std::thread`, `std::mutex`, `std::condition_variable`
- **Build System**: CMake

---

## 🔧 Building the Project


### Clone the repository
```bash
git clone https://github.com/yourusername/orderbook.git
cd orderbook
```

### Build and execute
```bash
cd src
make
./main
```
