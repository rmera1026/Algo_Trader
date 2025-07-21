# Test executable
add_executable(test_algo_trader
    test_indicators.cpp
    test_utils.cpp
    ../src/indicators.cpp
    ../src/utils.cpp
)

target_link_libraries(test_algo_trader 
    GTest::gtest 
    GTest::gtest_main
    ${CURL_LIBRARIES}
)

target_include_directories(test_algo_trader PRIVATE 
    ../src
    /opt/homebrew/include
)

# Register tests
add_test(NAME AlgoTraderTests COMMAND test_algo_trader)