#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "utils.h"
#include "indicators.h"
#include "strategy.h"

using json = nlohmann::json;

int main() {
    const char* apiKey = std::getenv("API_KEY");
    if (!apiKey) {
        std::cerr << "❌ API key not set. Please set the API_KEY environment variable.\n";
        return 1;
    }

    const std::string url = "https://financialmodelingprep.com/api/v3/historical-price-full/AAPL?serietype=line&apikey=" + std::string(apiKey);

    // Use the http_get function to fetch data
    std::string readBuffer = http_get(url);

    if (readBuffer.empty()) {
        std::cerr << "❌ Empty response received.\n";
        return 1;
    }

    json j;
    try {
        j = json::parse(readBuffer);
    } catch (json::parse_error& e) {
        std::cerr << "❌ JSON parse error: " << e.what() << "\n";
        return 1;
    }

    if (!j.contains("historical") || !j["historical"].is_array()) {
        std::cerr << "❌ Error: JSON does not contain valid 'historical' array.\n";
        return 1;
    }

    auto historical = j["historical"];
    if (historical.empty()) {
        std::cerr << "❌ Error: 'historical' data is empty.\n";
        return 1;
    }

    std::cout << "✅ Total records: " << historical.size() << "\n";

    // Parse closes from oldest to newest
    std::vector<double> closes;
    for (auto it = historical.rbegin(); it != historical.rend(); ++it)
        closes.push_back((*it)["close"]);

    const int MA_PERIOD = 200;
    const int RSI_PERIOD = 14;
    const int LOOK_AHEAD = 10;
    const double STOP_LOSS_PERCENT = 0.01;  // 1% below entry price
    const double TAKE_PROFIT_PERCENT = 0.02;  // 2% above entry price

    if (closes.size() < MA_PERIOD + LOOK_AHEAD + 1) {
        std::cerr << "❌ Not enough data: need at least " << MA_PERIOD + LOOK_AHEAD + 1 << " rows.\n";
        return 1;
    }

    auto sma200 = calc_sma(closes, MA_PERIOD);
    auto macd = calc_macd(closes);
    auto rsi = calc_rsi(closes, RSI_PERIOD);

    // Call the backtest_strategy function
    backtest_strategy(closes, sma200, rsi, macd.macd, macd.signal, LOOK_AHEAD, STOP_LOSS_PERCENT, TAKE_PROFIT_PERCENT);

    return 0;
}