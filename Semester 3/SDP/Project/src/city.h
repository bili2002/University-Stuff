// B)

#include "utilities.h"

class City {
    struct connection {
        int to;
        int time;
    };
    using landmarkConnections = std::vector<connection>;
    using allConnections = std::vector<landmarkConnections>;

    Utility::keysType keys;
    static const std::string startLandmark;

    std::string fileName;
    int k, r;
    int totalTime;

    allConnections con;

    void readFile();
    void findPathMaxLandmarksInTotaltime();

    std::vector<int> tempMaxLandmarksPath;
    int tempMax = -1;
    void dfs(int, int, int, std::vector<bool>&, std::vector<int>&);

public:
    std::vector<int> maxLandmarksPath;
    City(const std::string&);
    void printAnswer();
};