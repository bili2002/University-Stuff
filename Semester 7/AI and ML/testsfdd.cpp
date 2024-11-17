#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

struct Item {
    int weight;
    int value;
};

struct Individual {
    vector<int> genes;
    int fitness = 0;

    Individual() {}
    Individual(int num_items) : genes(num_items), fitness(0) {}

    void initialize_individual(std::mt19937& gen) {
        uniform_int_distribution<> dis(0, 1);

        for (auto& gene : genes) {
            gene = dis(gen);
        }
    }
};

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int population_size, int generations, double mutation_rate, int selection_size)
        : population_size(population_size), generations(generations), mutation_rate(mutation_rate), 
          selection_size(selection_size), gen(random_device{}()) {}

    void run(int new_capacity, const vector<Item>& new_items) {
        items = new_items;
        capacity = new_capacity;

        initialize_population(items.size());

        for (int gen = 0; gen < generations; ++gen) {
            cout << best_individual.fitness << endl; // if weight > capacity, fitness is 0
            evolve_population();
        }

        cout << best_individual.fitness << endl;
    }

private:
    int population_size;
    int generations;
    double mutation_rate;
    int selection_size;

    vector<Individual> population;
    Individual best_individual;
    
    vector<Item> items;
    int capacity;

    mutable std::mt19937 gen;

    void initialize_population(int num_items) {
        for (int i = 0; i < population_size; ++i) {
            Individual ind(num_items);
            ind.initialize_individual(gen);
            population.push_back(ind);
        }

        best_individual = population[0];
    }

    void evolve_population() {
        vector<Individual> new_population;

        for (auto& ind : population) {
            ind.fitness = calculate_fitness(ind);
            best_individual = ind.fitness > best_individual.fitness ? ind : best_individual;
        }

        while (new_population.size() < population_size) {
            Individual parent1 = selection();
            Individual parent2 = selection();

            auto [child1, child2] = crossover(parent1, parent2);

            mutate(child1);
            mutate(child2);

            child1.fitness = calculate_fitness(child1);
            child2.fitness = calculate_fitness(child2);

            new_population.push_back(child1);
            if (new_population.size() < population_size) {
                new_population.push_back(child2);
            }
        }

        swap(population, new_population);
    }

    int calculate_fitness(const Individual& ind) const {
        int total_weight = 0, total_value = 0;
        for (size_t i = 0; i < ind.genes.size(); ++i) {
            total_weight += ind.genes[i] * items[i].weight;
            total_value += ind.genes[i] * items[i].value;
        }

        return (total_weight <= capacity) ? total_value : 0;
    }

    Individual selection() const {
        uniform_int_distribution<> dis(0, population.size() - 1);

        int best_fitness = -1;
        Individual best_individual(population[0].genes.size());

        for (int i = 0; i < selection_size; ++i) {
            int idx = dis(gen);
            if (population[idx].fitness > best_fitness) {
                best_fitness = population[idx].fitness;
                best_individual = population[idx];
            }
        }
        return best_individual;
    }

    pair<Individual, Individual> crossover(const Individual& parent1, const Individual& parent2) const { // should try other crossover functions
        uniform_int_distribution<> dis(1, parent1.genes.size() - 2);

        int crossover_point = dis(gen);

        Individual child1(parent1.genes.size());
        Individual child2(parent2.genes.size());

        for (int i = 0; i < crossover_point; ++i) {
            child1.genes[i] = parent1.genes[i];
            child2.genes[i] = parent2.genes[i];
        }
        for (int i = crossover_point; i < parent1.genes.size(); ++i) {
            child1.genes[i] = parent2.genes[i];
            child2.genes[i] = parent1.genes[i];
        }

        return {child1, child2};
    }

    void mutate(Individual& ind) {
        uniform_real_distribution<> dis(0, 1);

        for (auto& gene : ind.genes) {
            gene = dis(gen) < mutation_rate ? 1 - gene : gene;
        }
    }
};

int main() {
    int capacity, num_items;
    cin >> capacity >> num_items;

    vector<Item> items(num_items);
    for (int i = 0; i < num_items; ++i) {
        cin >> items[i].weight >> items[i].value;
    }

    GeneticAlgorithm ga(1000, 100, 0.01, 5);
    ga.run(capacity, items);

    return 0;
}