#include "utilities.h"

void Utility::addKey(keysType& keys, const std::string& currStr) {
    auto id = keys.find(currStr);
    if (id != keys.end()) {
        return;
    }

    keys[currStr] = keys.size() - 1; /*zashtoto se smenq pri vikaneto na keys[currStr]*/
}

void Utility::createKeys(keysType& keys, const std::vector<std::string>& strings) {
    for (auto& curr : strings) {
        addKey(keys, curr);
    }
}

void Utility::createRevKeys(keysType& keys, revKeysType& revKeys) {
    for (auto& curr : keys) {
        revKeys[curr.second] = curr.first;
    }
}

std::vector<int> Utility::toKeys(keysType& keys, const std::vector<std::string>& vec) {
    std::vector<int> temp(vec.size());

    int cnt = 0;
    for (auto& curr : vec) {
        temp[cnt++] = keys[curr];
    }

    return temp;
}

void Utility::printAnswerPath(keysType& keys, const std::vector<int>& path) {
    revKeysType revKeys;
    Utility::createRevKeys(keys, revKeys);

    for (auto& curr : path) {
        std::cout<<revKeys[curr]<<' ';
    }
    std::cout<<std::endl;
}
