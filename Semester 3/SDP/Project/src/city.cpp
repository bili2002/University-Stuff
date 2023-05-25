#include "city.h"

const std::string City::startLandmark = "Railstation";

void City::readFile() {
    std::ifstream is(fileName);
    if (!is.is_open()) {
        throw std::runtime_error("Couldn't open file.");
    }

    is>>k>>r;
    con.resize(k);
    Utility::addKey(keys, startLandmark);

    for (int i=0; i<r; i++) {
        std::string landmark1Str, landmark2Str;
        int time;

        is>>landmark1Str>>landmark2Str>>time;

        Utility::addKey(keys, landmark1Str);
        Utility::addKey(keys, landmark2Str);

        int landmark1 = keys[landmark1Str];
        int landmark2 = keys[landmark2Str];
        con[landmark1].push_back({landmark2, time});
        con[landmark2].push_back({landmark1, time});
    }

    is>>totalTime;

    is.close();
}

void City::dfs(int curr, int currTime, int cnt, std::vector<bool>& bitmask, std::vector<int>& currPath) {
    if (currTime > totalTime) {
        return;
    }

    currPath.push_back(curr);
    if (curr == 0 && cnt > tempMax) {
        tempMaxLandmarksPath = currPath;
        tempMax = cnt;
    }

    for (auto& next : con[curr]) {
        if (bitmask[next.to]) {
            dfs(next.to, currTime + next.time, cnt, bitmask, currPath);
        } else {
            bitmask[next.to] = true;
            dfs(next.to, currTime + next.time, cnt+1, bitmask, currPath);
            bitmask[next.to] = false;
        }
    }

    currPath.pop_back(); //remove curr
}

void City::findPathMaxLandmarksInTotaltime() {
    std::vector<bool> tempBitmask(k, 0);
    std::vector<int> tempPath;

    dfs(0, 0, 0, tempBitmask, tempPath);

    maxLandmarksPath = tempMaxLandmarksPath;
}

City::City(const std::string& name) {  
    fileName = name;

    readFile();
    findPathMaxLandmarksInTotaltime();
}

void City::printAnswer() {
    Utility::printAnswerPath(keys, maxLandmarksPath);
}