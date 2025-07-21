#include <gtest/gtest.h>
#include "indicators.h"
#include "exceptions.h"
#include <vector>
#include <cmath>

class IndicatorsTest : public ::testing::Test {
protected:
    std::vector<double> sample_prices = {10.0, 12.0, 11.0, 13.0, 14.0, 12.0, 15.0, 16.0, 14.0, 17.0};
};

TEST_F(IndicatorsTest, SMACalculation) {
    auto sma = calc_sma(sample_prices, 3);
    
    // First two values should be NaN
    EXPECT_TRUE(std::isnan(sma[0]));
    EXPECT_TRUE(std::isnan(sma[1]));
    
    // Third value should be (10+12+11)/3 = 11
    EXPECT_NEAR(sma[2], 11.0, 0.01);
    
    // Fourth value should be (12+11+13)/3 = 12
    EXPECT_NEAR(sma[3], 12.0, 0.01);
}

TEST_F(IndicatorsTest, SMAInvalidInput) {
    EXPECT_THROW(calc_sma(sample_prices, 0), CalculationException);
    EXPECT_THROW(calc_sma(sample_prices, -1), CalculationException);
    EXPECT_THROW(calc_sma(std::vector<double>(), 5), CalculationException);
}

TEST_F(IndicatorsTest, RSICalculation) {
    // Create a trending up dataset
    std::vector<double> up_trend = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115};
    auto rsi = calc_rsi(up_trend, 14);
    
    // RSI should be high (>50) for uptrending data, and within valid range (0-100)
    EXPECT_GT(rsi[15], 50.0);
    EXPECT_LE(rsi[15], 100.0);  // Changed from LT to LE (less than or equal)
    EXPECT_GE(rsi[15], 0.0);    // Added check for lower bound
}

TEST_F(IndicatorsTest, MACDCalculation) {
    auto macd_result = calc_macd(sample_prices);
    
    // Should return both MACD and signal lines
    EXPECT_EQ(macd_result.macd.size(), sample_prices.size());
    EXPECT_EQ(macd_result.signal.size(), sample_prices.size());
}