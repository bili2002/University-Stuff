#include "ArrayProcessor.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <thread>
#include <utility>
#include <chrono>

const int ArrayProcessor::threadCount = 4;

ArrayProcessor::ArrayProcessor(const std::string& str) {
    parseArray(str);
}

void ArrayProcessor::mergeSortZones() {
    mergeSortZones(0, array.size(), threadCount);
}


void ArrayProcessor::mergeSortParallel() {
    mergeSortParallel(0, array.size());
}

void ArrayProcessor::merge(int left, int mid, int right) {
    int left_pointer = left;
    int right_pointer = mid;
    int new_pointer = 0;

    std::vector<int> new_array(right - left);

    while (new_pointer < right - left) {
        if (left_pointer == mid || (right_pointer < right && array[left_pointer] > array[right_pointer])) {
            new_array[new_pointer++] = array[right_pointer++];
        }
        else {
            new_array[new_pointer++] = array[left_pointer++];
        }
    }

    while (new_pointer--) {
        array[new_pointer + left] = new_array[new_pointer];
    }
}

void ArrayProcessor::mergeSortZones(int left, int right, int thread_number) {
    if (left + 1 == right) {
        return;
    }

    int mid = (left + right) / 2;

    if (thread_number > 1) { 
        std::thread left_thread([this, left, mid, thread_number]() { this->mergeSortZones(left, mid, thread_number / 2); });
        std::thread right_thread([this, mid, right, thread_number]() { this->mergeSortZones(mid, right, thread_number / 2); });

        left_thread.join();
        right_thread.join();
    }
    else {
        mergeSortZones(left, mid, thread_number);
        mergeSortZones(mid, right, thread_number);
    }

    merge(left, mid, right);
}

void ArrayProcessor::mergeSortParallel(int left, int right) {
    if (left + 1 == right) {
        return;
    }

    int mid = (left + right) / 2;

    std::thread left_thread([this, left, mid]() { this->mergeSortParallel(left, mid); });
    std::thread right_thread([this, mid, right]() { this->mergeSortParallel(mid, right); });

    left_thread.join();
    right_thread.join();

    merge(left, mid, right);
}

void ArrayProcessor::parseArray(const std::string& str) {
    array.push_back(0);

    for (auto ch : str) {
        if (ch == '\n' || ch == ' ') {
            array.push_back(0);
            continue;
        }
        array.back() = array.back() * 10 + (ch - '0');
    }

    array.pop_back();
}

std::string ArrayProcessor::getString() {
    std::string arrayString;
    
    for (auto num : array) {
        std::string numString;
        if (num == 0) {
            numString = "0";
        }
        else while (num) {
            numString += num % 10 + (int)'0';
            num /= 10;
        }

        std::reverse(numString.begin(), numString.end());
        arrayString += numString + " ";
    }

    arrayString.pop_back();
    arrayString += '\n';

    return arrayString;
}