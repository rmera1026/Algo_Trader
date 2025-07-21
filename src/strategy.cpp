#include "strategy.h"
#include "benchmark.h"
#include <iostream>

void backtest_strategy(const std::vector<double>& closes,
                       const std::vector<double>& sma200,
                       const std::vector<double>& rsi,
                       const std::vector<double>& macd,
                       const std::vector<double>& signal,
                       int look_ahead,
                       double stop_loss_percent,
                       double take_profit_percent) {
    
    BENCHMARK("Strategy Signal Detection");
    
    size_t triggers = 0, successes = 0;

    for (size_t i = 200; i + look_ahead < closes.size(); ++i) {
        bool above200 = closes[i] > sma200[i];
        bool bullishMACD = macd[i] > signal[i] && macd[i - 1] <= signal[i - 1];
        bool rsiBelow70 = rsi[i] < 70;

        if (above200 && bullishMACD && rsiBelow70) {
            ++triggers;

            double entryPrice = closes[i];
            double stopLoss = entryPrice * (1.0 - stop_loss_percent);
            double takeProfit = entryPrice * (1.0 + take_profit_percent);

            bool success = false;
            for (size_t j = i + 1; j <= i + look_ahead && j < closes.size(); ++j) {
                if (closes[j] >= takeProfit) {
                    success = true;
                    break;
                }
                if (closes[j] <= stopLoss) {
                    break;
                }
            }

            if (success) ++successes;
        }
    }

    double win_rate = triggers ? (100.0 * successes / triggers) : 0.0;

    std::cout << "\n📊 Strategy Results\n";
    std::cout << "  Triggers  : " << triggers << "\n";
    std::cout << "  Successes : " << successes << "\n";
    std::cout << "  Win Rate  : " << win_rate << "%\n";
}