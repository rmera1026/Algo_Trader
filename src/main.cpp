#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <limits>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Curl write callback
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Indicator helpers
double ema(double price, double prev_ema, int period) {
    double k = 2.0 / (period + 1.0);
    return price * k + prev_ema * (1.0 - k);
}

std::vector<double> calc_sma(const std::vector<double>& v, int period) {
    std::vector<double> out(v.size(), std::numeric_limits<double>::quiet_NaN());
    if (v.size() < period) return out;

    double sum = std::accumulate(v.begin(), v.begin() + period, 0.0);
    out[period - 1] = sum / period;
    for (size_t i = period; i < v.size(); ++i) {
        sum += v[i] - v[i - period];
        out[i] = sum / period;
    }
    return out;
}

struct MACD { std::vector<double> macd, signal; };

MACD calc_macd(const std::vector<double>& c, int fast=12, int slow=26, int sig=9) {
    std::vector<double> macd(c.size()), ema_fast(c.size()), ema_slow(c.size());
    if (c.empty()) return {macd, macd};

    ema_fast[0] = ema_slow[0] = c[0];
    for (size_t i = 1; i < c.size(); ++i) {
        ema_fast[i] = ema(c[i], ema_fast[i - 1], fast);
        ema_slow[i] = ema(c[i], ema_slow[i - 1], slow);
        macd[i]     = ema_fast[i] - ema_slow[i];
    }

    std::vector<double> signal(c.size());
    signal[0] = macd[0];
    for (size_t i = 1; i < c.size(); ++i)
        signal[i] = ema(macd[i], signal[i - 1], sig);

    return {macd, signal};
}

// RSI calculation
std::vector<double> calc_rsi(const std::vector<double>& closes, int period = 14) {
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

int main() {
    const char* apiKey = std::getenv("API_KEY");
    if (!apiKey) {
        std::cerr << "❌ API key not set. Please set the API_KEY environment variable.\n";
        return 1;
    }
    const std::string url = "https://financialmodelingprep.com/api/v3/historical-price-full/AAPL?serietype=line&apikey=" + std::string(apiKey);

    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (!curl) {
        std::cerr << "❌ Failed to initialize CURL.\n";
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "❌ curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        return 1;
    }

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

    size_t triggers = 0, successes = 0;
    for (size_t i = MA_PERIOD; i + LOOK_AHEAD < closes.size(); ++i) {
        bool above200 = closes[i] > sma200[i];
        bool bullishMACD = macd.macd[i] > macd.signal[i] &&
                           macd.macd[i - 1] <= macd.signal[i - 1];
        bool rsiBelow70 = rsi[i] < 70;

        if (above200 && bullishMACD && rsiBelow70) {
            ++triggers;

            double entryPrice = closes[i];
            double stopLoss = entryPrice * (1.0 - STOP_LOSS_PERCENT);
            double takeProfit = entryPrice * (1.0 + TAKE_PROFIT_PERCENT);

            bool success = false;
            for (size_t j = i + 1; j <= i + LOOK_AHEAD && j < closes.size(); ++j) {
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

    std::cout << "\n📊 Strategy Results (With Risk-Reward Ratios)\n";
    std::cout << "  Triggers  : " << triggers << "\n";
    std::cout << "  Successes : " << successes << "\n";
    std::cout << "  Win Rate  : " << win_rate << "%\n";

    return 0;
}