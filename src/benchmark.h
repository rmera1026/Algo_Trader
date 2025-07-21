#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string name;

public:
    explicit Timer(const std::string& timer_name) : name(timer_name) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "⏱️  " << std::left << std::setw(25) << name 
                  << ": " << std::right << std::setw(8) << duration.count() << " μs\n";
    }
};

#define BENCHMARK(name) Timer timer(name)

#endif // BENCHMARK_H