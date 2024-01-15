#pragma once
#include <vector> 
#include <string>
#include <mutex>
#include <stack>

class ArrayProcessor {
public:
    ArrayProcessor(const std::string& str);
    void mergeSort();

private:
    static const int threadCount;
    
    std::vector<int> array;
    
    void merge(int left, int mid, int right);
    void mergeSort(int left, int right);
    void parseArray(const std::string& str);
};