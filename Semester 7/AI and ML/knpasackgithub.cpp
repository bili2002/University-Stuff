
#include <iostream>

using namespace std;
typedef long long ll;

const int S = 5000, G = 1, N = 200;

vector<vector<bool> > chromosomes, offspring;
int n, s, items_volume[N], items_benefit[N], fitness[S], total_fitness;
unordered_map<int, int> fitness_cnt;

void generate_population() {
    chromosomes.resize(S);
    for (int i = 0; i < S; ++i) {
        chromosomes[i].resize(n);
        for (int j = 0; j < n; ++j) {
            int r = rand() % 10;
            if (r < 5)
                chromosomes[i][j] = 1;
        }
    }
}

void calc_fitness() {
    total_fitness = 0;
    fitness_cnt.clear();
    for (int i = 0; i < S; ++i) {
        int total_benefit = 0, total_volume = 0;
        for (int j = 0; j < n; ++j) {
            if (chromosomes[i][j]) {
                total_volume += items_volume[j];
                total_benefit += items_benefit[j];
            }
        }
        if (total_volume > s) {
            while (total_volume > s) {
                int r = rand() % n;
                while (chromosomes[i][r] == 0)
                    r = rand() % n;
                chromosomes[i][r] = 0;
                total_volume -= items_volume[r];
                total_benefit -= items_benefit[r];
            }
        }
        fitness[i] = total_benefit;
        ++fitness_cnt[fitness[i]];
        total_fitness += fitness[i];
    }
}

int calc_percentage() {
    int max_cnt = 0;
    for (auto x: fitness_cnt)
        max_cnt = max(max_cnt, x.second);
    return max_cnt * 100 / S;
}

int get_fittest() {
    int max_fitness = INT_MIN, index = -1;
    for (int i = 0; i < S; ++i) {
        if (max_fitness < fitness[i]) {
            max_fitness = fitness[i];
            index = i;
        }
    }
    return index;
}

int get_second_fittest(int fittest_index) {
    int max_second_fitness = INT_MIN, index = -1;
    for (int i = 0; i < S; ++i) {
        if (i != fittest_index && max_second_fitness < fitness[i]) {
            max_second_fitness = fitness[i];
            index = i;
        }
    }
    return index;
}

void elitism() {
    int fittest_index = get_fittest();
    offspring.push_back(chromosomes[fittest_index]);
    int second_fittest_index = get_second_fittest(fittest_index);
    offspring.push_back(chromosomes[second_fittest_index]);
}

int roulette_wheel_selection() {
    int r = rand() % total_fitness, sum = 0;
    for (int i = 0; i < S; ++i) {
        sum += fitness[i];
        if (sum > r)
            return i;
    }
    return -1;
}

void mutation(vector<bool> &chromosome) {
    for (int i = 0; i < n; ++i) {
        int r = rand() % 1000;
        if (r < 1)
            chromosome[i] = !chromosome[i];
    }
}

void crossover(int chromosome_index1, int chromosome_index2) {
    vector<bool> chromosome1 = chromosomes[chromosome_index1];
    vector<bool> chromosome2 = chromosomes[chromosome_index2];
    int r = rand() % n;
    for (int i = 0; i < r; ++i)
        swap(chromosome1[i], chromosome2[i]);
    mutation(chromosome1);
    mutation(chromosome2);
    offspring.push_back(chromosome1);
    offspring.push_back(chromosome2);
}

void reproduce() {
    // Elitism - two of the fittest chromosomes are copied without changes to a new population
    elitism();
    while (offspring.size() < S) {
        int chromosome_index1 = roulette_wheel_selection();
        int chromosome_index2 = roulette_wheel_selection();
        crossover(chromosome_index1, chromosome_index2);
    }
}

int main() {
    srand(time(NULL));
    int t=1;
    for (int c = 1; c <= t; ++c) {
        offspring.clear();
        cin >> s >> n;
        for (int i = 0; i < n; ++i)
            cin >> items_volume[i] >> items_benefit[i];
        generate_population();
        int percentage = 0, generation_number = 0;
        while (percentage < 90 || generation_number < G) {
            if (generation_number) {
                chromosomes = offspring;
                offspring.clear();
            }
            calc_fitness();
            percentage = calc_percentage();
            ++generation_number;
            // think if the percentage will be lost !!
            if (percentage < 90 || generation_number < G)
                reproduce();
        }
        int fittest_index = get_fittest();
        int res = fitness[fittest_index];
        cout << "Case: " << c << ' ' << res << '\n';
    }
    return 0;
}