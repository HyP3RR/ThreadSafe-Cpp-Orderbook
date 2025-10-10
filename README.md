# Thread-Safe C++ Orderbook

An ongoing implementation of a thread-safe orderbook in C++. The project focuses on core trading system mechanics such as order matching, execution handling, and safe concurrent access.

---

## 🚀 Features
- **Order Types**: Supports GTC (Good Till Cancel), FAK (Fill And Kill), and Market Orders.
- **Matching Engine**: Implements price–time priority with support for partial fills and immediate execution.
- **Thread Safety**: Dedicated orderbook writer thread with thread-safe queues ensures reliable and consistent request/response handling.
- **Execution Handling**: Robust handling of add, cancel, and modify operations.

---
## Directory Structure
```bash
ThreadSafe-Cpp-Orderbook/
├── Makefile                  # Build File
├── README.md                 # Project documentation
├── LICENSE                   # Project license
├── include/                  # Header files
│   ├── constants.hpp         # Basic changeable constants
│   ├── helper.hpp            # helper functions declarations 
│   ├── order.hpp             # Order class declaration
│   ├── orderbook.hpp         # OrderBook class declaration 
│   └── trades.hpp            # Client helper functions
├── src/                      # Source files
│   ├── helper.cpp            # Client helper functions
│   ├── order.cpp             # Order class implementation
│   ├── orderbook.cpp         # Orderbook class implementation
└── tests/                    # Test scenarios
    ├── test_scenario1.cpp    # Test scenario 1
    ├── test_scenario2.cpp    # Test scenario 2
    └── ...                   # Additional test scenarios
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

Each test source file lives in `tests/file_name.cpp`. Compiled executables are placed in the `build/` folder.

From the project root, run:
```bash
make build/file_name
./build/file_name
```
