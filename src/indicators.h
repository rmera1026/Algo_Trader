#ifndef INDICATORS_H
#define INDICATORS_H

#include <vector>

// SMA calculation
std::vector<double> calc_sma(const std::vector<double>& v, int period);

// MACD calculation
struct MACD { std::vector<double> macd, signal; };
MACD calc_macd(const std::vector<double>& c, int fast = 12, int slow = 26, int sig = 9);

// RSI calculation
std::vector<double> calc_rsi(const std::vector<double>& closes, int period = 14);

#endif // INDICATORS_H