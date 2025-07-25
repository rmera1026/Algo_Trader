#include "indicators.h"
#include "exceptions.h"
#include "benchmark.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>

// SMA calculation
std::vector<double> calc_sma(const std::vector<double>& v, int period) {
    // Uncomment the following line to enable benchmarking
    // BENCHMARK("SMA Internal Calculation");
    
    if (period <= 0) {
        throw CalculationException("SMA period must be positive");
    }
    if (v.empty()) {
        throw CalculationException("Cannot calculate SMA on empty data");
    }

    std::vector<double> out(v.size(), std::numeric_limits<double>::quiet_NaN());
    if (v.size() < static_cast<size_t>(period)) return out;

    for (size_t i = period - 1; i < v.size(); ++i) {
        double sum = 0.0;
        for (int j = 0; j < period; ++j) {
            sum += v[i - j];
        }
        out[i] = sum / period;
    }
    return out;
}

// MACD calculation
MACD calc_macd(const std::vector<double>& closes, int fast, int slow, int signal_period) {
    // Uncomment the following line to enable benchmarking
    // BENCHMARK("MACD Internal Calculation");

    std::vector<double> macd(closes.size()), ema_fast(closes.size()), ema_slow(closes.size());
    if (closes.empty()) return {macd, macd};

    ema_fast[0] = ema_slow[0] = closes[0];
    for (size_t i = 1; i < closes.size(); ++i) {
        double k_fast = 2.0 / (fast + 1.0);
        double k_slow = 2.0 / (slow + 1.0);
        ema_fast[i] = closes[i] * k_fast + ema_fast[i - 1] * (1.0 - k_fast);
        ema_slow[i] = closes[i] * k_slow + ema_slow[i - 1] * (1.0 - k_slow);
        macd[i] = ema_fast[i] - ema_slow[i];
    }

    std::vector<double> signal(closes.size());
    signal[0] = macd[0];
    for (size_t i = 1; i < closes.size(); ++i) {
        double k_sig = 2.0 / (signal_period + 1.0);
        signal[i] = macd[i] * k_sig + signal[i - 1] * (1.0 - k_sig);
    }

    return {macd, signal};
}

// RSI calculation
std::vector<double> calc_rsi(const std::vector<double>& closes, int period) {
    // Uncomment the following line to enable benchmarking
    // BENCHMARK("RSI Internal Calculation");

    std::vector<double> rsi(closes.size(), std::numeric_limits<double>::quiet_NaN());
    if (closes.size() < period + 1) return rsi;

    double gain = 0.0, loss = 0.0;
    for (size_t i = 1; i <= period; ++i) {
        double change = closes[i] - closes[i - 1];
        if (change > 0) gain += change;
        else loss -= change;
    }

    gain /= period;
    loss /= period;
    rsi[period] = 100.0 - (100.0 / (1.0 + (gain / loss)));

    for (size_t i = period + 1; i < closes.size(); ++i) {
        double change = closes[i] - closes[i - 1];
        if (change > 0) {
            gain = (gain * (period - 1) + change) / period;
            loss = (loss * (period - 1)) / period;
        } else {
            gain = (gain * (period - 1)) / period;
            loss = (loss * (period - 1) - change) / period;
        }
        rsi[i] = 100.0 - (100.0 / (1.0 + (gain / loss)));
    }

    return rsi;
}