#include "MatrixProcessor.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <thread>
#include <chrono>

const std::vector<std::pair<int, int>> MatrixProcessor::directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
const int MatrixProcessor::threadCount = 16;

MatrixProcessor::MatrixProcessor(const std::string& str) {
    parseMatrix(str);
}

int MatrixProcessor::countZones() {
    int cnt = 0;

    for (int i=0; i<matrix.size(); i++) {
        for (int j=0; j<matrix[i].size(); j++) {
            if (matrix[i][j]) {
                cnt++;
                dfs(i, j);
            }
        }
    }

    return cnt;
}

void MatrixProcessor::parseMatrix(const std::string& str) {
    matrix.emplace_back();

    for (auto ch : str) {
        if (ch == '\n') {
            matrix.emplace_back();
            continue;
        }
        matrix.back().push_back(ch - '0');
    }

    matrix.pop_back();
}

bool MatrixProcessor::inBound(int i, int j) const {
    return i >= 0 && i < matrix.size() && j >= 0 && j < matrix[0].size();
}

void MatrixProcessor::dfsInner(std::stack<std::pair<int, int>>& dfsStack) {
    while (true) {
        mtx.lock();

        if (dfsStack.empty()) {
            mtx.unlock();
            break;
        }
        auto curr = dfsStack.top();
        dfsStack.pop();

        mtx.unlock();

        for (auto dir : directions) {
            int i = curr.first + dir.first;
            int j = curr.second + dir.second;
            
            if (!inBound(i, j)) {
                continue;
            }

            mtx.lock();

            if (matrix[i][j]) {
                matrix[i][j] = false;
                dfsStack.push({i, j});
            }
            
            mtx.unlock();
        }
    }
}

void MatrixProcessor::dfs(int i, int j) {
    std::stack<std::pair<int, int>> dfsStack;
    dfsStack.push({i, j});

    std::vector<std::thread> threads;

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([&]() {
            dfsInner(dfsStack);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}