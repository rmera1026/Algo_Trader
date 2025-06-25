#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// Store API response in a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    // What I want:
    // To backtest a strategy.
    // The strategy is to look for a time where:
    // - The price of a stock was over the 200MA
    // - RSI was oversold
    // - Then the MACD line crossed over the signal line
    // Then, see if price goes up after that most of the time.
    // Output the success percentage of that strategy.

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string apiKey = "POLYGON_API_KEY";
    std::string symbol = "AAPL";
    std::string url = "https://api.polygon.io/v2/last/trade/" + symbol + "?apiKey=" + apiKey;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            try {
                json data = json::parse(readBuffer);
                std::cout << "Price: " << data["results"]["p"] << std::endl;
                std::cout << "Time: " << data["results"]["t"] << std::endl;
            } catch (json::parse_error& e) {
                std::cerr << "Parse error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "curl error: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return 0;
}