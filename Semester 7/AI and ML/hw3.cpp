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
    Individual(int num_items) : genes(num_items, 0) {}
};

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int ps, int g, int ss, double cr, double mr, double mgr)
        : POPULATION_SIZE(ps), GENERATIONS(g), MUTATION_RATE(mr), 
          SELECTION_SIZE(ss), CROSSOVER_RATE(cr), MUTATION_GENE_RATE(mgr), 
          gen(random_device{}()) {}

    void run(int new_capacity, const vector<Item>& new_items) {
        items = new_items;
        capacity = new_capacity;

        initialize_population();

        for (int gen = 0; gen < GENERATIONS; ++gen) {
            cout << best_individual.fitness << endl; // if weight > capacity, fitness is 0
            evolve_population();
        }

        cout << best_individual.fitness << endl;
    }

private:
    int POPULATION_SIZE;
    int GENERATIONS;
    int SELECTION_SIZE;
    
    double MUTATION_RATE;
    double CROSSOVER_RATE;
    double MUTATION_GENE_RATE;

    vector<Individual> population;
    Individual best_individual;
    
    vector<Item> items;
    int capacity;

    mutable std::mt19937 gen;

    // For each individual selects random genes, preserverving the total weight below the capacity 
    void initialize_population() {
        uniform_real_distribution<> dis(0, 1);

        for (int i = 0; i < POPULATION_SIZE; ++i) {
            Individual ind(items.size());
            std::vector<int> permutation = generate_random_permutation(items.size());

            int total_weight = 0;
            for (int& j : permutation) {
                if (total_weight + items[j].weight > capacity) {
                    continue;
                }

                ind.genes[j] = 1;
                total_weight += items[j].weight;
            }

            ind.fitness = calculate_fitness(ind);
            population.push_back(ind);

            best_individual = ind.fitness > best_individual.fitness ? ind : best_individual;
        }
    }

    void evolve_population() { 
        vector<Individual> new_population;

        for (auto& ind : population) {
            ind.fitness = calculate_fitness(ind);
            best_individual = ind.fitness > best_individual.fitness ? ind : best_individual;
        }

        while (new_population.size() < POPULATION_SIZE) {
            auto [parent1, parent2] = selection();
            auto [child1, child2] = crossover(parent1, parent2);

            mutate(child1);
            mutate(child2);

            fix_individual(child1);
            fix_individual(child1);

            child1.fitness = calculate_fitness(child1);
            child2.fitness = calculate_fitness(child2);
            
            new_population.push_back(child1);
            if (new_population.size() < POPULATION_SIZE) {
                new_population.push_back(child2);
            }
        }

        swap(population, new_population);
    }

    // Returns total value; if the capacity is exceded returns 0 instead
    int calculate_fitness(const Individual& ind) const { 
        int total_weight = 0, total_value = 0;
        for (size_t i = 0; i < ind.genes.size(); ++i) {
            total_weight += ind.genes[i] * items[i].weight;
            total_value += ind.genes[i] * items[i].value;
        }

        return (total_weight <= capacity) ? total_value : 0;
    }


    // Selects the 2 best out of randomly selected SELECTION_SIZE
    pair<Individual, Individual> selection() const { 
        uniform_int_distribution<> dis(0, population.size() - 1);

        Individual best_individual;
        Individual best_individual_second;

        for (int i = 0; i < SELECTION_SIZE; ++i) {
            int idx = dis(gen);
            if (population[idx].fitness >= best_individual.fitness) {
                best_individual_second = best_individual;
                best_individual = population[idx];
            }
            else if (population[idx].fitness >= best_individual_second.fitness) {
                best_individual_second = population[idx];
            } 
        }
        return {best_individual, best_individual_second};
    }

    // Does two point crossover
    pair<Individual, Individual> crossover(const Individual& parent1, const Individual& parent2) const { 
        uniform_real_distribution<> dis_real(0, 1);
        if (dis_real(gen) > CROSSOVER_RATE) {
            return {parent1, parent2};
        }

        uniform_int_distribution<> dis(1, parent1.genes.size() - 2);

        int point1 = dis(gen);
        int point2 = dis(gen);

        if (point1 > point2) std::swap(point1, point2);

        Individual child1(parent1.genes.size());
        Individual child2(parent2.genes.size());

        for (int i = 0; i < point1; ++i) {
            child1.genes[i] = parent1.genes[i];
            child2.genes[i] = parent2.genes[i];
        }
        for (int i = point1; i <= point2; ++i) {
            child1.genes[i] = parent2.genes[i];
            child2.genes[i] = parent1.genes[i];
        }
        for (int i = point2 + 1; i < parent1.genes.size(); ++i) {
            child1.genes[i] = parent1.genes[i];
            child2.genes[i] = parent2.genes[i];
        }

        return {child1, child2};
    }

    void mutate(Individual& ind) {
        uniform_real_distribution<> dis(0, 1);
        if (dis(gen) > MUTATION_RATE) {
            return;
        }

        for (auto& gene : ind.genes) {
            gene = dis(gen) > MUTATION_GENE_RATE ? gene : 1 - gene;
        }
    }

    // Randomly select used genes and removes them until the total weight is under the capacity
    void fix_individual(Individual& ind) { 
        int total_weight = 0;

        for (size_t i = 0; i < ind.genes.size(); ++i) {
            total_weight += ind.genes[i] * items[i].weight;
        }

        if (total_weight > capacity) {
            vector<int> perm = generate_random_permutation(items.size());

            for (int& i : perm) {
                if (total_weight <= capacity) {
                    break;
                }

                if (ind.genes[i]) {
                    ind.genes[i] = 0;
                    total_weight -= items[i].weight;
                }
            }
        }
    }

    std::vector<int> generate_random_permutation(int n) {
        std::vector<int> permutation(n);
        
        for (int i = 0; i < n; ++i) {
            permutation[i] = i;
        }
        
        std::shuffle(permutation.begin(), permutation.end(), gen);
        return permutation;
    }
};

int main() {
    int capacity, num_items;
    cin >> capacity >> num_items;

    vector<Item> items(num_items);
    for (int i = 0; i < num_items; ++i) {
        cin >> items[i].weight >> items[i].value;
    }

    GeneticAlgorithm ga(1500, 50, 5, 0.7, 0.3, 0.01);
    ga.run(capacity, items);

    return 0;
}
