#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <curl/curl.h>

// Function to perform an HTTP GET request and return the response as a string
std::string http_get(const std::string& url);

// Curl write callback function
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

#endif // UTILS_H