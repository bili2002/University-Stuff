#include "ArrayProcessor.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <thread>
#include <chrono>

const int ArrayProcessor::threadCount = 4;

ArrayProcessor::ArrayProcessor(const std::string& str) {
    parseArray(str);
}

void ArrayProcessor::mergeSort() {
    mergeSort(0, array.size());
}

void ArrayProcessor::merge(int left, int mid, int right) {
    int left_pointer = left;
    int right_pointer = mid;
    int new_pointer = 0;

    std::vector<int> new_array(right - left);

    while (new_pointer < right - left) {
        if (left_pointer == mid || array[left_pointer] < array[right_pointer]) {
            new_array[new_pointer++] = array[left_pointer];
            left_pointer++;
        }
        else {
            new_array[new_pointer++] = array[right_pointer];
            right_pointer++;
        }
    }

    while (new_pointer--) {
        array[new_pointer + left] = new_array[new_pointer];
    }
}

void ArrayProcessor::mergeSort(int left, int right) {
    if (left == right) {
        return;
    }

    int mid = (left + right) / 2;
    mergeSort(left, mid);
    mergeSort(mid, right);
}

void ArrayProcessor::parseArray(const std::string& str) {
    array.emplace_back();

    for (auto ch : str) {
        if (ch == '\n') {
            array.emplace_back();
            continue;
        }
        array.back() = array.back() * 10 + (ch - '0');
    }

    array.pop_back();
}