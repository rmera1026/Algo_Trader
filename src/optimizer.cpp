#include "optimizer.h"
#include "indicators.h"
#include "benchmark.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

void StrategyParameters::mutate(std::mt19937& gen, double mutation_rate) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> ma_dist(50, 300);
    std::uniform_int_distribution<> rsi_dist(10, 20);
    std::uniform_real_distribution<> threshold_dist(60.0, 80.0);
    std::uniform_real_distribution<> percent_dist(0.005, 0.05);
    std::uniform_int_distribution<> look_dist(5, 20);
    
    if (dis(gen) < mutation_rate) ma_period = ma_dist(gen);
    if (dis(gen) < mutation_rate) rsi_period = rsi_dist(gen);
    if (dis(gen) < mutation_rate) rsi_threshold = threshold_dist(gen);
    if (dis(gen) < mutation_rate) stop_loss = percent_dist(gen);
    if (dis(gen) < mutation_rate) take_profit = percent_dist(gen);
    if (dis(gen) < mutation_rate) look_ahead = look_dist(gen);
}

StrategyParameters StrategyParameters::crossover(const StrategyParameters& parent1, 
                                               const StrategyParameters& parent2, 
                                               std::mt19937& gen) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    StrategyParameters child;
    
    child.ma_period = dis(gen) < 0.5 ? parent1.ma_period : parent2.ma_period;
    child.rsi_period = dis(gen) < 0.5 ? parent1.rsi_period : parent2.rsi_period;
    child.rsi_threshold = dis(gen) < 0.5 ? parent1.rsi_threshold : parent2.rsi_threshold;
    child.stop_loss = dis(gen) < 0.5 ? parent1.stop_loss : parent2.stop_loss;
    child.take_profit = dis(gen) < 0.5 ? parent1.take_profit : parent2.take_profit;
    child.look_ahead = dis(gen) < 0.5 ? parent1.look_ahead : parent2.look_ahead;
    
    return child;
}

GeneticOptimizer::GeneticOptimizer(size_t pop_size, int max_gen, double mut_rate, double elite)
    : population_size(pop_size), max_generations(max_gen), mutation_rate(mut_rate), 
      elite_ratio(elite), gen(std::random_device{}()) {}

OptimizationResult GeneticOptimizer::optimize(
    const std::vector<double>& prices,
    std::function<double(const std::vector<double>&, const StrategyParameters&)> fitness_func) {
    
    BENCHMARK("Genetic Algorithm Optimization");
    
    // Initialize population
    std::vector<StrategyParameters> population(population_size);
    std::vector<double> fitness(population_size);
    
    // Random parameter ranges
    std::uniform_int_distribution<> ma_dist(50, 300);
    std::uniform_int_distribution<> rsi_dist(10, 20);
    std::uniform_real_distribution<> threshold_dist(60.0, 80.0);
    std::uniform_real_distribution<> percent_dist(0.005, 0.05);
    std::uniform_int_distribution<> look_dist(5, 20);
    
    // Create initial population
    for (auto& individual : population) {
        individual.ma_period = ma_dist(gen);
        individual.rsi_period = rsi_dist(gen);
        individual.rsi_threshold = threshold_dist(gen);
        individual.stop_loss = percent_dist(gen);
        individual.take_profit = percent_dist(gen);
        individual.look_ahead = look_dist(gen);
    }
    
    OptimizationResult result;
    result.best_fitness = -1e6;
    
    std::cout << "\n🧬 Starting Genetic Algorithm Optimization...\n";
    std::cout << "Population: " << population_size << ", Generations: " << max_generations << "\n\n";
    
    for (int generation = 0; generation < max_generations; ++generation) {
        // Evaluate fitness for each individual
        for (size_t i = 0; i < population_size; ++i) {
            fitness[i] = fitness_func(prices, population[i]);
        }
        
        // Find best individual
        auto best_it = std::max_element(fitness.begin(), fitness.end());
        size_t best_idx = std::distance(fitness.begin(), best_it);
        
        if (fitness[best_idx] > result.best_fitness) {
            result.best_fitness = fitness[best_idx];
            result.best_params = population[best_idx];
        }
        
        result.fitness_history.push_back(*best_it);
        
        // Print progress every 10 generations
        if (generation % 10 == 0 || generation == max_generations - 1) {
            std::cout << "Generation " << std::setw(3) << generation 
                      << " | Best Fitness: " << std::fixed << std::setprecision(2) 
                      << *best_it << " | Avg: " 
                      << std::accumulate(fitness.begin(), fitness.end(), 0.0) / population_size 
                      << "\n";
        }
        
        // Selection and reproduction
        std::vector<StrategyParameters> new_population;
        
        // Keep elite individuals
        std::vector<size_t> indices(population_size);
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), 
                  [&](size_t a, size_t b) { return fitness[a] > fitness[b]; });
        
        size_t elite_count = static_cast<size_t>(population_size * elite_ratio);
        for (size_t i = 0; i < elite_count; ++i) {
            new_population.push_back(population[indices[i]]);
        }
        
        // Generate offspring
        std::uniform_int_distribution<size_t> parent_dist(0, elite_count - 1);
        while (new_population.size() < population_size) {
            size_t parent1_idx = indices[parent_dist(gen)];
            size_t parent2_idx = indices[parent_dist(gen)];
            
            auto child = StrategyParameters::crossover(population[parent1_idx], 
                                                     population[parent2_idx], gen);
            child.mutate(gen, mutation_rate);
            new_population.push_back(child);
        }
        
        population = std::move(new_population);
    }
    
    result.generations = max_generations;
    
    std::cout << "\n🎯 Optimization Complete!\n";
    std::cout << "Best Parameters:\n";
    std::cout << "  MA Period: " << result.best_params.ma_period << "\n";
    std::cout << "  RSI Period: " << result.best_params.rsi_period << "\n";
    std::cout << "  RSI Threshold: " << result.best_params.rsi_threshold << "\n";
    std::cout << "  Stop Loss: " << result.best_params.stop_loss * 100 << "%\n";
    std::cout << "  Take Profit: " << result.best_params.take_profit * 100 << "%\n";
    std::cout << "  Look Ahead: " << result.best_params.look_ahead << " days\n";

    // Calculate and display actual optimized performance
    auto opt_result = backtest_detailed(prices, result.best_params);
    std::cout << "\n📊 Optimized Strategy Performance:\n";
    std::cout << "  Triggers: " << opt_result.triggers << "\n";
    std::cout << "  Successes: " << opt_result.successes << "\n";
    std::cout << "  Win Rate: " << std::fixed << std::setprecision(2) << opt_result.win_rate << "%\n";
    std::cout << "  Fitness Score: " << result.best_fitness << "\n\n";
    
    return result;
}

// Detailed backtest function that returns full results

BacktestResult backtest_detailed(const std::vector<double>& prices, const StrategyParameters& params) {
    BacktestResult result{-1000.0, 0.0, 0, 0};
    
    if (prices.size() < params.ma_period + params.look_ahead + 50) {
        return result;
    }
    
    try {
        auto sma = calc_sma(prices, params.ma_period);
        auto macd = calc_macd(prices);
        auto rsi = calc_rsi(prices, params.rsi_period);
        
        size_t triggers = 0, successes = 0;
        double total_return = 0.0;
        
        for (size_t i = params.ma_period; i + params.look_ahead < prices.size(); ++i) {
            bool above_ma = prices[i] > sma[i];
            bool bullish_macd = macd.macd[i] > macd.signal[i] && 
                               macd.macd[i-1] <= macd.signal[i-1];
            bool rsi_condition = rsi[i] < params.rsi_threshold;
            
            if (above_ma && bullish_macd && rsi_condition) {
                ++triggers;
                
                double entry_price = prices[i];
                double stop_loss = entry_price * (1.0 - params.stop_loss);
                double take_profit = entry_price * (1.0 + params.take_profit);
                
                for (size_t j = i + 1; j <= i + params.look_ahead && j < prices.size(); ++j) {
                    if (prices[j] >= take_profit) {
                        ++successes;
                        total_return += params.take_profit;
                        break;
                    }
                    if (prices[j] <= stop_loss) {
                        total_return -= params.stop_loss;
                        break;
                    }
                }
            }
        }
        
        if (triggers == 0) {
            result.fitness = -100.0;
            return result;
        }
        
        double win_rate = static_cast<double>(successes) / triggers;
        double avg_return = total_return / triggers;
        
        result.fitness = win_rate * 100.0 + avg_return * 1000.0;
        result.win_rate = win_rate * 100.0;
        result.triggers = triggers;
        result.successes = successes;
        
        return result;
        
    } catch (...) {
        return result;
    }
}

// Original function for compatibility
double backtest_fitness(const std::vector<double>& prices, const StrategyParameters& params) {
    return backtest_detailed(prices, params).fitness;
}