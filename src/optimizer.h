#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include <functional>
#include <random>

struct StrategyParameters {
    int ma_period = 200;
    int rsi_period = 14;
    double rsi_threshold = 70.0;
    double stop_loss = 0.01;
    double take_profit = 0.02;
    int look_ahead = 10;
    
    // For genetic algorithm
    void mutate(std::mt19937& gen, double mutation_rate = 0.1);
    static StrategyParameters crossover(const StrategyParameters& parent1, 
                                       const StrategyParameters& parent2, 
                                       std::mt19937& gen);
};

struct OptimizationResult {
    StrategyParameters best_params;
    double best_fitness;
    std::vector<double> fitness_history;
    int generations;
};

class GeneticOptimizer {
private:
    size_t population_size;
    int max_generations;
    double mutation_rate;
    double elite_ratio;
    std::mt19937 gen;
    
public:
    GeneticOptimizer(size_t pop_size = 50, int max_gen = 100, 
                    double mut_rate = 0.1, double elite = 0.2);
    
    OptimizationResult optimize(
        const std::vector<double>& prices,
        std::function<double(const std::vector<double>&, const StrategyParameters&)> fitness_func);
};

// Fitness function for backtesting
double backtest_fitness(const std::vector<double>& prices, const StrategyParameters& params);

#endif // OPTIMIZER_H