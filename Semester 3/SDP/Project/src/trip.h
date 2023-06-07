// A)

#pragma once
#include <unordered_map>
#include "skip_list.h"
#include "utilities.h"

class Trip {
    Utility::keysType keys;

    int n;
    SkipList<int> cities;
    std::unordered_map<int, SkipListIterator<int>> citiesPtrs; //find pointer in list from key
    
    void createBaseList();
    void createCitiesPtrs();
    void addDirectLinks(const std::vector<std::pair<std::string, std::string>>&);
    void findPathMinThroughWantedCities(const std::vector<int>&);

    void backTrackPath(const std::vector<std::pair<int, int>>&);

    static const int INF;

public:
    std::vector<int> minCitiesPath;
    Trip(int, const std::vector<std::string>&, const std::vector<std::pair<std::string, std::string>>&, const std::vector<std::string>&);
    void printAnswer();
};