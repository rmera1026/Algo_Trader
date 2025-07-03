#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Curl write callback to store response in std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = (std::string*)userp;
    str->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://financialmodelingprep.com/api/v3/quote-short/AAPL?apikey=lDiWKSE6uPheOC2yVQ9JjUdlX6OZtNaM");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            curl_easy_cleanup(curl);
            return 1;
        }
        curl_easy_cleanup(curl);

        std::cout << "Raw JSON response:\n" << readBuffer << "\n";

        try {
            json data = json::parse(readBuffer);

            if (!data.is_array() || data.empty()) {
                std::cerr << "Unexpected JSON format: expected a non-empty array.\n";
                return 1;
            }

            // The API returns an array with one object
            json& quote = data[0];

            // Extract some useful fields
            if(quote.contains("symbol") && quote.contains("price") && quote.contains("volume")) {
                std::cout << "Symbol: " << quote["symbol"] << "\n";
                std::cout << "Price: " << quote["price"] << "\n";
                std::cout << "Volume: " << quote["volume"] << "\n";
            } else {
                std::cerr << "JSON object missing expected keys.\n";
            }

        } catch (json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << "\n";
            return 1;
        }

    } else {
        std::cerr << "Failed to initialize curl.\n";
        return 1;
    }

    return 0;
}