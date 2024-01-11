#pragma once
#include <vector> 
#include <string>
#include <mutex>
#include <stack>

class MatrixProcessor {
public:
    MatrixProcessor(const std::string& str);
    int countZones();

private:
    static const int threadCount;
    static const std::vector<std::pair<int, int>> directions;

    std::mutex mtx;
    std::vector<std::vector<bool>> matrix;
    
    void parseMatrix(const std::string& str);

    bool inBound(int i, int j) const;
    void dfs(int i, int j);
    void dfsInner(std::stack<std::pair<int, int>>& st);
};