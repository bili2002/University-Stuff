#include <algorithm>
#include <mutex>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <future>

#define gassert assert

using namespace std::chrono;

struct GASettings {
    int populationSize = 2000;
    float eliteKeep = 0.05f;
    float mutationRate = 0.03f;
    float mutationPower = 0.1f;
};


struct ConsoleRenderer {
    ConsoleRenderer() = default;

    void clear() {
        const auto now = high_resolution_clock::now();

        ++frameCounter;
        if (duration_cast<milliseconds>(now - lastSecond) > 1000ms) {
            lastSecond = now;
            averageFPS = frameCounter - lastSecondFrames;
            lastSecondFrames = frameCounter;
        }
        const int lastFrameFPS = 1s / duration_cast<microseconds>(now - lastFrame);
        lastFrame = now;
        printf("\033[%d;%dH", 0, 0);
        std::cout << std::setfill(' ') << std::setw(clearCount) << ' ';
        printf("\033[%d;%dH", 0, 0);

        clearCount = 0;
        renderText("FPS: ");
        renderText(lastFrameFPS);
        renderText(", frames for last second: ");
        renderText(averageFPS);
        newLine();
    }

    void renderText(const char *text) {
        lineLength += std::strlen(text);
        std::cout << text;
    }
    void renderText(int value) {
        lineLength += int(log10(value) + 1);
        std::cout << value;
    }

    void newLine() {
        struct winsize w;
        
        // Get the terminal size
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
            perror("ioctl");
            throw;
        }
        clearCount += ((lineLength / w.ws_col) + 1) * w.ws_col;

        lineLength = 0;
        std::cout << std::endl;
    }

private:
    int averageFPS = 0;
    int lastSecondFrames = 0;
    int frameCounter = 0;
    int clearCount = 0;
    int lineLength = 0;

    high_resolution_clock::time_point lastFrame;
    high_resolution_clock::time_point lastSecond;
};

struct TargetCheck {
private:
    std::string target;
    std::string alphabet;
    mutable std::uniform_int_distribution<int> basePick;
public:
    TargetCheck(const std::string &desired) {
        char symbols[] = "_!@#$%^&*()<>[];:'\" ";
        alphabet.reserve(256);
        for (int c = 'a'; c <= 'z'; c++) {
            alphabet.push_back(char(c));
        }
        for (int c = 'A'; c <= 'Z'; c++) {
            alphabet.push_back(char(c));
        }
        for (int c = '0'; c <= '9'; c++) {
            alphabet.push_back(char(c));
        }

        for (int c = 0; c < std::size(symbols); c++) {
            alphabet.push_back(symbols[c]);
        }
        basePick = std::uniform_int_distribution<int>(0, int(alphabet.size() - 1));
        const bool accepted = setTarget(desired);
        assert(accepted);
    }

    int getTargetSize() const {
        gassert(!target.empty());
        return int(target.size());
    }

    int distance(const std::string &str) const {
        gassert(!target.empty());
        if (str.size() != target.size()) {
            gassert(str.size() != target.size());
            return -1;
        }
        int sum = 0;
        for (int c = 0; c < int(target.size()); c++) {
            const int diff = target[c] - str[c];
            sum += diff * diff;
        }
        gassert(sum >= 0);
        return sum;
    }

    char newBase(std::mt19937 &rng) const {
        return alphabet[basePick(rng)];
    }
private:
    bool setTarget(const std::string &desired) {
        for (int c = 0; c < int(desired.size()); c++) {
            if (alphabet.find(desired[c]) == std::string::npos) {
                return false;
            }
        }
        target = desired;
        return true;
    }
};

struct GARunner {
private:
    const size_t FPS_TARGET = 60;

    using UniformIntRange = std::uniform_int_distribution<int>;
    using UniformFloatRange = std::uniform_real_distribution<float>;
    struct Individual {
        std::string dna;
        int error = -1;
    };

    std::vector<Individual> generation;

    std::random_device rd;
    mutable std::vector<std::mt19937> rng;

    GASettings gaSettings;
    const TargetCheck &checker;
    ConsoleRenderer renderer;
    int genPerSecond = 0;
    float completed = 0.f;

    size_t threadsNumber;

    high_resolution_clock::time_point lastRenderTime;

public:
    GARunner(const GASettings &settings, const TargetCheck &targetCheck, size_t threadsNumber = 1) : checker(targetCheck) {
        gaSettings = settings;
        generation.resize(settings.populationSize, Individual{ std::string(targetCheck.getTargetSize(), ' ') });
        this->threadsNumber = threadsNumber;

        for (auto i = 0; i < threadsNumber; i++) {
            rng.push_back(std::move(std::mt19937(rd())));
        }
    }

    void runFor(int generationCount) {
        gassert(int(generation.size()) == gaSettings.populationSize && "Must be initialized");

        high_resolution_clock::time_point tick = high_resolution_clock::now();
        lastRenderTime = high_resolution_clock::now();

        int lastGen = 0;
        for (int c = 0; generationCount == -1 || c < generationCount; c++) {
            high_resolution_clock::time_point tock = high_resolution_clock::now();
            if (duration_cast<milliseconds>(tock - tick) > 500ms) {
                genPerSecond = (c - lastGen) * 2;
                lastGen = c;
                tick = tock;
            }
            completed = float(c + 1) / generationCount;
            

            // some things are not parallel because crossover was taking 90% from execution time so they are mostly irrelenant
            // they may even be slower if creating new threads

            // I think because of this - it turned out my thread pool implementation was slower (not 100% sure)
            prepare();
            render();
            crossover();
            mutate();
        }
        prepare();
    }

private:
    void render() {
        if (high_resolution_clock::now() - lastRenderTime < 1000ms / FPS_TARGET) {
            return;
        }   

        renderer.clear();
        for (int c = 0; c < 3; c++) {
            renderer.renderText(generation[c].dna.c_str());
            renderer.renderText(" ");
            renderer.renderText(generation[c].error);
            renderer.newLine();
        }
        renderer.renderText("Generations per second: ");
        renderer.renderText(genPerSecond);
        renderer.renderText(" completed: ");
        renderer.renderText((completed * 10000) / 100);
        renderer.renderText("%");
        renderer.newLine();

        lastRenderTime = high_resolution_clock::now();
    }

    int getEliteCount() const {
        const int eliteCount = int(gaSettings.eliteKeep * float(generation.size()));
        return std::max(1, eliteCount);
    }

    void prepare() {
        auto subDist = [&](int startIndex, int endIndex) {
            for (int c = startIndex; c < endIndex; c++) {
                generation[c].error = checker.distance(generation[c].dna);
            }
        };

        std::vector<std::thread> threads;
        const int chuckArray = generation.size() / threadsNumber;

        for (int i = 0; i < threadsNumber; ++i) {
            const int startIndex = chuckArray * i;
            const int endIndex = i != threadsNumber - 1 ? chuckArray * (i + 1) : generation.size();
            threads.push_back(std::move(std::thread(subDist, startIndex, endIndex)));
        }

        for (auto& th : threads) {
            th.join();
        }

        // slower when parallel
        std::sort(generation.begin(), generation.end(), [](const Individual &a, const Individual &b) {
            return a.error < b.error;
        });
    }

    Individual crossover(int left, int right, int idx) const {
        UniformIntRange parentPick(0, 1);
        const Individual *parents[2] = { &generation[left], &generation[right] };
        Individual newIndividual{ std::string(checker.getTargetSize(), ' ') };

        for (int c = 0; c < int(newIndividual.dna.size()); c++) {
            newIndividual.dna[c] = parents[parentPick(rng[idx])]->dna[c];
        }
        return newIndividual;
    }

    void crossover() {
        std::vector<Individual> newGeneration;
        newGeneration.resize(generation.size());

        const int eliteCount = getEliteCount();
        for (int c = 0; c < eliteCount; c++) {
            newGeneration[c] = generation[c];
        }
        
        auto subCrossover = [&](int startIndex, int endIndex, int idx) {
            thread_local UniformIntRange parentPick(0, int(generation.size() - 1));

            for (int c = startIndex; c < endIndex; c++) {
                const int left = parentPick(rng[idx]);
                const int right = parentPick(rng[idx]);

                const Individual child = crossover(left, right, idx);
                newGeneration[c] = child;
            }
        };

        std::vector<std::thread> threads;
        const int chuckArray = (generation.size() - eliteCount) / threadsNumber;

        for (int i = 0; i < threadsNumber; ++i) {
            const int startIndex = chuckArray * i + eliteCount;
            const int endIndex = i != threadsNumber - 1 ? chuckArray * (i + 1) + eliteCount : generation.size();
            
            threads.push_back(std::move(std::thread(subCrossover, startIndex, endIndex, i)));
        }

        for (auto& th : threads) {
            th.join();
        }

        generation.swap(newGeneration);
    }

    void mutate(Individual &individual, int idx) {
        UniformFloatRange chanceMutate(0.f, 1.f);

        for (int c = 0; c < int(individual.dna.size()); c++) {
            if (chanceMutate(rng[idx]) < gaSettings.mutationPower) {
                individual.dna[c] = checker.newBase(rng[idx]);
            }
        }
    }

    void mutate() {
        const int eliteCount = getEliteCount();

        auto subMutate = [&](int startIndex, int endIndex, int idx) {
            thread_local UniformFloatRange mutateChance(0.f, 1.f);

            for (int c = startIndex; c < endIndex; c++) {
                if (mutateChance(rng[idx]) < gaSettings.mutationRate) {
                    mutate(generation[c], idx);
                }
            }
        };

        std::vector<std::thread> threads;
        const int chuckArray = (generation.size() - eliteCount) / threadsNumber;

        for (int i = 0; i < threadsNumber; ++i) {
            const int startIndex = chuckArray * i + eliteCount;
            const int endIndex = i != threadsNumber - 1 ? chuckArray * (i + 1) + eliteCount : generation.size();
            threads.push_back(std::move(std::thread(subMutate, startIndex, endIndex, i)));
        }

        for (auto& th : threads) {
            th.join();
        }
    }
};

#include <thread>
int main() {
    const auto start = high_resolution_clock::now();
    const GASettings settings;
    TargetCheck target("std::cout << duration_cast<minutes>(runtime) << ' ' << duration_cast<seconds>(runtime) % 60 << ' ' << duration_cast<milliseconds>(runtime) % 1000 << ' ' << duration_cast<microseconds>(runtime) % 1000;");
    GARunner ga(settings, target, 16);

    ga.runFor(20'000);

    const auto runtime = high_resolution_clock::now() - start;
    // std::cout << duration_cast<minutes>(runtime) << ' ' << duration_cast<seconds>(runtime) % 60 << ' ' << duration_cast<milliseconds>(runtime) % 1000 << ' ' << duration_cast<microseconds>(runtime) % 1000;
    return 0;
}