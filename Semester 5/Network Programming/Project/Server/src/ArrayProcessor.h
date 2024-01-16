#pragma once
#include <vector> 
#include <string>
#include <stack>

class ArrayProcessor {
public:
    ArrayProcessor(const std::string& str);
    void mergeSortZones();
    void mergeSortParallel();

    std::string getString();

private:
    static const int threadCount;
    
    std::vector<int> array;
    
    void merge(int left, int mid, int right);
    void mergeSortZones(int left, int right, int min_zone);
    void mergeSortParallel(int left, int right);
    void parseArray(const std::string& str);
};