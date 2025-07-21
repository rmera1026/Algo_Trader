#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "utils.h"
#include "indicators.h"
#include "strategy.h"
#include "exceptions.h"

using json = nlohmann::json;

int main() {
    try {
        const char* apiKey = std::getenv("API_KEY");
        if (!apiKey) {
            throw APIException("API key not set. Please set the API_KEY environment variable.");
        }

        std::string symbol;
        std::cout << "Enter stock symbol: ";
        std::cin >> symbol;

        const std::string url = "https://financialmodelingprep.com/api/v3/historical-price-full/" + symbol + "?serietype=line&apikey=" + std::string(apiKey);

        // Use the http_get function to fetch data
        std::string readBuffer = http_get(url);

        if (readBuffer.empty()) {
            throw APIException("Empty response received from API");
        }

        json j;
        try {
            j = json::parse(readBuffer);
        } catch (json::parse_error& e) {
            throw DataException("JSON parse error: " + std::string(e.what()));
        }

        if (!j.contains("historical") || !j["historical"].is_array()) {
            throw DataException("JSON does not contain valid 'historical' array");
        }

        auto historical = j["historical"];
        if (historical.empty()) {
            throw DataException("Historical data is empty");
        }

        std::cout << "✅ Total records: " << historical.size() << "\n";

        // Parse closes from oldest to newest
        std::vector<double> closes;
        int skipped_count = 0;
        for (auto it = historical.rbegin(); it != historical.rend(); ++it) {
            if (it->contains("close") && !(*it)["close"].is_null()) {
                closes.push_back((*it)["close"]);
            } else if (it->contains("Close") && !(*it)["Close"].is_null()) {
                closes.push_back((*it)["Close"]);
            } else {
                skipped_count++;
                continue;
            }
        }

        if (skipped_count > 0) {
            std::cout << "⚠️ Skipped " << skipped_count << " records due to missing close prices\n";
        }

        if (closes.size() < 250) {
            throw DataException("Insufficient valid data points: " + std::to_string(closes.size()) + " (need at least 250)");
        }

        std::cout << "✅ Valid records: " << closes.size() << "\n";

        // For strategies that need high/low, use close prices as approximation
        std::vector<double> highs = closes;
        std::vector<double> lows = closes;

        const int MA_PERIOD = 200;
        const int RSI_PERIOD = 14;
        const int LOOK_AHEAD = 10;
        const double STOP_LOSS_PERCENT = 0.01;
        const double TAKE_PROFIT_PERCENT = 0.02;

        auto sma200 = calc_sma(closes, MA_PERIOD);
        auto macd = calc_macd(closes);
        auto rsi = calc_rsi(closes, RSI_PERIOD);

        // Validate indicator calculations
        if (sma200.empty() || macd.macd.empty() || rsi.empty()) {
            throw CalculationException("Failed to calculate technical indicators");
        }

        backtest_strategy(closes, sma200, rsi, macd.macd, macd.signal, LOOK_AHEAD, STOP_LOSS_PERCENT, TAKE_PROFIT_PERCENT);

        return 0;

    } catch (const TradingException& e) {
        std::cerr << "❌ Trading Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "❌ Unexpected Error: " << e.what() << "\n";
        return 1;
    }
}