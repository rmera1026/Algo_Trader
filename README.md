# Algorithmic Trading System with AI Optimization

A high-performance C++17 algorithmic trading system featuring technical analysis indicators, AI-powered parameter optimization, and comprehensive backtesting capabilities.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)
![AI](https://img.shields.io/badge/AI-Genetic%20Algorithm-orange.svg)
![Tests](https://img.shields.io/badge/Tests-Google%20Test-red.svg)

## 🚀 Features

- **Real-time Market Data Integration**: Financial Modeling Prep API integration
- **Technical Analysis**: SMA, MACD, RSI indicators with validated calculations
- **AI Parameter Optimization**: Genetic algorithm for automatic strategy tuning
- **Backtesting Engine**: Strategy performance analysis with risk/reward metrics
- **Performance Profiling**: Microsecond-level timing and benchmarking
- **Robust Error Handling**: Custom exception hierarchy for different error types
- **Comprehensive Testing**: Unit test suite using Google Test framework
- **Cross-Platform Build**: CMake build system for portability

## 🧬 AI Optimization Engine

The system uses a **Genetic Algorithm** to automatically discover optimal trading parameters:

- **Population Size**: 30 parameter combinations
- **Generations**: 50 evolution cycles
- **Optimization Target**: Win rate and average return
- **Parameters Optimized**: MA period, RSI thresholds, stop-loss/take-profit levels

**Example Optimization Results:**
```
Original Strategy: 43.75% win rate
Optimized Strategy: 69.87% win rate
Improvement: +26.12%
```

## 📊 Trading Strategy

**Entry Conditions:**
- Price above N-day Simple Moving Average (uptrend confirmation)
- MACD line crosses above signal line (momentum shift)
- RSI below threshold (avoiding overbought conditions)

**Exit Conditions:**
- Take Profit: Configurable % gain from entry price
- Stop Loss: Configurable % loss from entry price
- Look-ahead period: Configurable trading days

## 🛠️ Installation

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

## 💻 Usage

### Set up API Key
```bash
export API_KEY=your_financial_modeling_prep_api_key
```

### Run the Application
```bash
./AlgoTrader
```

### Example Session with AI Optimization
```
Enter stock symbol: AAPL
✅ Total records: 5000
✅ Valid records: 5000
⏱️  All Indicator Calculations:    5791 μs
⏱️  Strategy Signal Detection:     4782 μs

📊 Strategy Results
  Triggers  : 157
  Successes : 89
  Win Rate  : 56.69%

🤖 Running Parameter Optimization...
Run genetic algorithm optimization? (y/n): y

🧬 Starting Genetic Algorithm Optimization...
Generation  10 | Best Fitness: 52.18 | Avg: 47.32
Generation  20 | Best Fitness: 58.91 | Avg: 54.12
Generation  50 | Best Fitness: 69.87 | Avg: 66.45

🎯 Optimization Complete!
Best Parameters:
  MA Period: 165
  RSI Threshold: 67.5
  Stop Loss: 0.8%
  Take Profit: 2.3%
```

## 🏗️ Project Architecture

```
algo_trader/
├── src/
│   ├── main.cpp          # Entry point, user interface, data orchestration
│   ├── indicators.cpp    # Technical analysis (SMA, MACD, RSI)
│   ├── indicators.h      # Technical indicator function declarations
│   ├── strategy.cpp      # Trading logic and backtesting engine
│   ├── strategy.h        # Strategy function declarations
│   ├── optimizer.cpp     # Genetic algorithm implementation
│   ├── optimizer.h       # Optimizer class and parameter definitions
│   ├── utils.cpp         # HTTP client and API integration
│   ├── utils.h           # Utility function declarations
│   ├── benchmark.h       # Performance timing and profiling
│   └── exceptions.h      # Custom error handling classes
├── tests/
│   ├── test_indicators.cpp # Unit tests for technical indicators
│   ├── test_utils.cpp      # Unit tests for utility functions
│   └── CMakeLists.txt      # Test build configuration
├── build/                  # Build output directory (gitignored)
├── CMakeLists.txt          # Cross-platform build configuration
├── README.md               # Project documentation
└── .gitignore             # Git ignore patterns
```

## ⚡ Performance

- **Data Processing**: 10,000+ historical points in milliseconds
- **Indicator Calculations**: Optimized STL algorithms with O(n) complexity
- **AI Optimization**: 1,500 backtests in ~2 seconds (30 pop × 50 gen)
- **Memory Efficient**: Minimal allocation with move semantics
- **Benchmarked Operations**: Microsecond-level performance monitoring

## 🧠 Technical Highlights

### AI & Machine Learning
- **Genetic Algorithm**: Population-based optimization with crossover and mutation
- **Fitness Function**: Multi-objective optimization (win rate + returns)
- **Parameter Space**: 6-dimensional continuous and discrete optimization
- **Convergence Analysis**: Statistical tracking of population improvement

### Software Engineering
- **Modern C++17**: Smart pointers, STL algorithms, move semantics
- **Error Safety**: RAII principles and exception-safe design
- **Modular Architecture**: Separation of concerns with clean interfaces
- **Performance Profiling**: Built-in timing infrastructure

## 🔬 Future Enhancements

- [ ] Multi-threading for parallel genetic algorithm populations
- [ ] Advanced indicators (Bollinger Bands, Sharpe Ratio)
- [ ] Real-time WebSocket data streaming
- [ ] Machine learning integration (neural networks for signal prediction)
- [ ] Portfolio optimization with risk management
- [ ] GitHub Actions CI/CD pipeline

## 📈 Resume Highlights

This project demonstrates:
- **AI/ML Algorithm Implementation** (Genetic Algorithms)
- **Financial Domain Expertise** (Trading Strategies, Technical Analysis)
- **High-Performance C++** (Optimization, Memory Management)
- **Software Architecture** (Modular Design, Testing, Build Systems)
- **Mathematical Programming** (Statistical Analysis, Optimization Theory)

## 📄 License

MIT License - See LICENSE file for details.
