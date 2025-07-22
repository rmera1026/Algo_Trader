cmake_minimum_required(VERSION 3.16)
project(AlgoTrader VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)

# Include directories
include_directories(src)
include_directories(/opt/homebrew/include)

# Source files
set(SOURCES
    src/main.cpp
    src/utils.cpp
    src/indicators.cpp
    src/strategy.cpp
    src/optimizer.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${CURL_CFLAGS_OTHER})

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)

# Enable testing
enable_testing()

# Google Test (we'll add this next)
find_package(GTest QUIET)
if(GTest_FOUND)
    add_subdirectory(tests)
endif()