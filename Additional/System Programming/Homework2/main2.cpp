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
const    size_t threadsNumber = 4;

struct GARunner {
private:
    using UniformIntRange = std::uniform_int_distribution<int>;
    using UniformFloatRange = std::uniform_real_distribution<float>;
    struct Individual {
        std::string dna;
        int error = -1;
    };

    std::vector<Individual> generation;

    mutable std::mt19937 rng{ 42 };

    GASettings gaSettings;
    const TargetCheck &checker;
    ConsoleRenderer renderer;
    int genPerSecond = 0;
    float completed = 0.f;


public:
    GARunner(const GASettings &settings, const TargetCheck &targetCheck, size_t threadsNumber = 1) : checker(targetCheck) {
        gaSettings = settings;
        generation.resize(settings.populationSize, Individual{ std::string(targetCheck.getTargetSize(), ' ') });
    }

    void runFor(int generationCount) {
        gassert(int(generation.size()) == gaSettings.populationSize && "Must be initialized");

        high_resolution_clock::time_point tick = high_resolution_clock::now();
        int lastGen = 0;
        for (int c = 0; generationCount == -1 || c < generationCount; c++) {
            high_resolution_clock::time_point tock = high_resolution_clock::now();
            if (duration_cast<milliseconds>(tock - tick) > 500ms) {
                genPerSecond = (c - lastGen) * 2;
                lastGen = c;
                tick = tock;
            }
            completed = float(c + 1) / generationCount;
            prepare();
            render();
            crossover();
            mutate();
        }
        prepare();
    }

private:

    void render() {
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

    }

    int getEliteCount() const {
        const int eliteCount = int(gaSettings.eliteKeep * float(generation.size()));
        return std::max(1, eliteCount);
    }

    void prepare() {
        std::array<std::thread, threadsNumber> threads;
        const int chuckArray = generation.size() / threadsNumber;

        auto subSum = [&](int threadIndex, int startIndex, int endIndex) {
            for (int c = 0; c < int(generation.size()); c++) {
                generation[c].error = checker.distance(generation[c].dna);
            }
        };

        for (int i = 0; i < threadsNumber; ++i) {
            const int startIndex = chuckArray * i;
            const int endIndex = i != threadsNumber - 1 ? chuckArray * (i + 1) : generation.size();
            threads[i] = std::thread(subSum, i, startIndex, endIndex);
        }

        for (int i = 0; i < threadsNumber; ++i) {
            threads[i].join();
        }

        std::sort(generation.begin(), generation.end(), [](const Individual &a, const Individual &b) {
            return a.error < b.error;
        });
    }

    Individual crossover(int left, int right) const {
        const Individual *parents[2] = { &generation[left], &generation[right] };
        Individual newIndividual{ std::string(checker.getTargetSize(), ' ') };
        UniformIntRange parentPick(0, 1);
        for (int c = 0; c < int(newIndividual.dna.size()); c++) {
            newIndividual.dna[c] = parents[parentPick(rng)]->dna[c];
        }
        return newIndividual;
    }

    void crossover() {
        std::vector<Individual> newGeneration;
        newGeneration.reserve(generation.size());
        int c = 0;
        const int eliteCount = getEliteCount();
        for (; c < eliteCount; c++) {
            newGeneration.push_back(generation[c]);
        }

        UniformIntRange parentPick(0, int(generation.size() - 1));
        for (; c < generation.size(); c++) {
            const int left = parentPick(rng);
            const int right = parentPick(rng);
            const Individual child = crossover(left, right);
            newGeneration.push_back(child);
        }

        generation.swap(newGeneration);
    }

    void mutate(Individual &individual) {
        UniformFloatRange chanceMutate(0.f, 1.f);
        for (int c = 0; c < int(individual.dna.size()); c++) {
            if (chanceMutate(rng) < gaSettings.mutationPower) {
                individual.dna[c] = checker.newBase(rng);
            }
        }
    }

    void mutate() {
        UniformFloatRange mutateChance(0.f, 1.f);
        const int eliteCount = getEliteCount();
        for (int c = eliteCount; c < int(generation.size()); c++) {
            if (mutateChance(rng) < gaSettings.mutationRate) {
                mutate(generation[c]);
            }
        }
    }
};

#include <thread>
int main() {
    const auto start = high_resolution_clock::now();
    const GASettings settings;
    TargetCheck target("std::cout << duration_cast<minutes>(runtime) << ' ' << duration_cast<seconds>(runtime) % 60 << ' ' << duration_cast<milliseconds>(runtime) % 1000 << ' ' << duration_cast<microseconds>(runtime) % 1000;");
    GARunner ga(settings, target);

    ga.runFor(20'000);

    const auto runtime = high_resolution_clock::now() - start;
    // std::cout << duration_cast<minutes>(runtime) << ' ' << duration_cast<seconds>(runtime) % 60 << ' ' << duration_cast<milliseconds>(runtime) % 1000 << ' ' << duration_cast<microseconds>(runtime) % 1000;
    return 0;
}