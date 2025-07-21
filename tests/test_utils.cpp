#include <gtest/gtest.h>
#include "utils.h"
#include "exceptions.h"

TEST(UtilsTest, HTTPGetValidURL) {
    // Test with a simple HTTP endpoint
    std::string result = http_get("https://httpbin.org/get");
    EXPECT_FALSE(result.empty());
    EXPECT_TRUE(result.find("httpbin") != std::string::npos);
}

TEST(UtilsTest, HTTPGetInvalidURL) {
    // Test with invalid URL
    std::string result = http_get("invalid-url");
    EXPECT_TRUE(result.empty());
}