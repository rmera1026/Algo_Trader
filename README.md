# Algorithmic Trading System

A high-performance C++17 algorithmic trading system featuring technical analysis indicators, backtesting capabilities, and comprehensive error handling.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)
![Tests](https://img.shields.io/badge/Tests-Google%20Test-red.svg)

## Features

- **Real-time Market Data Integration**: Financial Modeling Prep API integration
- **Technical Analysis**: SMA, MACD, RSI indicators with validated calculations
- **Backtesting Engine**: Strategy performance analysis with risk/reward metrics
- **Robust Error Handling**: Custom exception hierarchy for different error types
- **Comprehensive Testing**: Unit test suite using Google Test framework
- **Cross-Platform Build**: CMake build system for portability
- **Performance Monitoring**: Built-in timing and benchmarking capabilities

## Strategy Overview

The system implements a trend-following momentum strategy:

**Entry Conditions:**
- Price above 200-day Simple Moving Average (uptrend confirmation)
- MACD line crosses above signal line (momentum shift)
- RSI below 70 (avoiding overbought conditions)

**Exit Conditions:**
- Take Profit: 2% gain from entry price
- Stop Loss: 1% loss from entry price
- Look-ahead period: 10 trading days

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+)
- CMake 3.16 or higher
- libcurl development libraries
- Google Test (for running tests)
- nlohmann/json library

## Installation

### macOS
```bash
# Install dependencies
brew install cmake curl googletest nlohmann-json

# Clone repository
git clone https://github.com/yourusername/algo_trader.git
cd algo_trader

# Build
mkdir build && cd build
cmake ..
make

# Run tests
make test
```

## Usage

### Set up API Key
```bash
export API_KEY=your_financial_modeling_prep_api_key
```

### Run the Application
```bash
./AlgoTrader
```

### Example Session
```
Enter stock symbol: AAPL
⏱️  HTTP API Request         :    45782 μs
⏱️  Data Fetching & Parsing  :    47923 μs
✅ Total records: 5000
✅ Valid records: 5000
⏱️  SMA Calculation         :     1247 μs
⏱️  MACD Calculation        :     2891 μs
⏱️  RSI Calculation         :     1653 μs
⏱️  All Indicator Calculations:    5791 μs
⏱️  Strategy Signal Detection:     4782 μs

📊 Strategy Results
  Triggers  : 157
  Successes : 89
  Win Rate  : 56.69%
⏱️  Total Execution Time    :    58496 μs
```

## Project Structure

```
algo_trader/
├── src/
│   ├── main.cpp          # Application entry point and data processing
│   ├── indicators.cpp    # Technical analysis implementations
│   ├── indicators.h      # Technical indicator function declarations
│   ├── strategy.cpp      # Trading strategy logic and backtesting
│   ├── strategy.h        # Strategy function declarations
│   ├── utils.cpp         # HTTP utilities for API communication
│   ├── utils.h           # Utility function declarations
│   ├── exceptions.h      # Custom exception class definitions
│   └── benchmark.h       # Performance timing utilities
├── tests/
│   ├── test_indicators.cpp # Unit tests for technical indicators
│   ├── test_utils.cpp      # Unit tests for utility functions
│   └── CMakeLists.txt      # Test build configuration
├── CMakeLists.txt          # Main build configuration
├── README.md               # Project documentation
└── .gitignore             # Git ignore patterns
```

## Performance

- Processes 10,000+ data points in milliseconds
- Memory-optimized indicator calculations using STL algorithms
- Efficient backtesting with O(n) complexity for most operations
- Comprehensive error handling with minimal performance overhead

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature-name`
3. Add comprehensive tests for new functionality
4. Ensure all tests pass: `make test`
5. Follow existing code style and conventions
6. Submit a pull request with detailed description

## License

This project is licensed under the MIT License.
