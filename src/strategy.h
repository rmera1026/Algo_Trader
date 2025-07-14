#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>

// Function to backtest the trading strategy
void backtest_strategy(const std::vector<double>& closes,
                       const std::vector<double>& sma200,
                       const std::vector<double>& rsi,
                       const std::vector<double>& macd,
                       const std::vector<double>& signal,
                       int look_ahead,
                       double stop_loss_percent,
                       double take_profit_percent);

#endif // STRATEGY_H