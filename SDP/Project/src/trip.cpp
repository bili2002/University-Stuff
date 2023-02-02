#include <algorithm>
#include "trip.h"

Trip::Trip(int nTemp, const std::vector<std::string>& citiesString, const std::vector<std::pair<std::string, std::string>>& directLinks, const std::vector<std::string>& wantedCitiesStr) {
    n = nTemp;

    Utility::createKeys(keys, citiesString);

    createBaseList();
    createCitiesPtrs();

    addDirectLinks(directLinks);

    findPathMinThroughWantedCities(Utility::toKeys(keys, wantedCitiesStr));
}

void Trip::createBaseList() {
    for (int i=0; i<n; i++) {
        cities.push_back(i);
    }
}

void Trip::createCitiesPtrs() {
    for (SkipListIterator<int> id = cities.begin(); id != cities.end(); id++) {
        citiesPtrs[*id] = id;
    }
}

void Trip::addDirectLinks(const std::vector<std::pair<std::string, std::string>>& directLinks) {
    for (auto& curr : directLinks) {
        int from = keys[curr.first];
        auto fromPtr = citiesPtrs[from];

        int to = keys[curr.second];
        auto toPtr = citiesPtrs[to];

        fromPtr.setSkip(toPtr);
    }
}

const int Trip::INF = 1e9;

void Trip::findPathMinThroughWantedCities(const std::vector<int>& wantedCities) {
    std::vector<std::pair<int, int>> dp(n, {INF, INF}); //min cities, last city

    auto wh = wantedCities.begin(); //pointer to show which wanted city is next

    dp[0] = {1, 0};
    for (SkipListIterator<int> currPtr = cities.begin(); currPtr != cities.end(); currPtr++)  {
        int curr = *currPtr;

        if (wh != wantedCities.end() && curr == *wh) {
            wh++;
        }

        if (currPtr.getSkip().isValid()) {
            int skip = *currPtr.getSkip();

            if ((wh != wantedCities.end() && skip <= *wh) && dp[skip].first > dp[curr].first + 1) {
                dp[skip] = {dp[curr].first + 1, curr};

            }
        }

        if (currPtr.getNext().isValid()) {
            int next = *currPtr.getNext();            

            if (dp[next].first > dp[curr].first + 1) {
                dp[next] = {dp[curr].first + 1, curr};

            }
        }      
    }

    backTrackPath(dp);
}

void Trip::backTrackPath(const std::vector<std::pair<int, int>>& dp) {
    int curr = n-1;

    while (dp[curr].second != curr) {
        minCitiesPath.push_back(curr);
        curr = dp[curr].second;
    }
    minCitiesPath.push_back(curr);

    std::reverse(minCitiesPath.begin(), minCitiesPath.end());
}

void Trip::printAnswer() {
    Utility::printAnswerPath(keys, minCitiesPath);
}