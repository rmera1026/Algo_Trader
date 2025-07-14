#include "utils.h"
#include <iostream>
#include <string>
#include <curl/curl.h>

// Curl write callback function
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function to perform an HTTP GET request and return the response as a string
std::string http_get(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (!curl) {
        std::cerr << "❌ Failed to initialize CURL.\n";
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "❌ curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        return "";
    }

    return readBuffer;
}