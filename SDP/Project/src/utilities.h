#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

namespace Utility {
    using keysType = std::unordered_map<std::string, int>;
    using revKeysType = std::unordered_map<int, std::string>;
    using pathType = std::vector<int>;

    void addKey(keysType&, const std::string&);
    void createKeys(keysType&, const std::vector<std::string>&);
    void createRevKeys(keysType&, revKeysType&);
    std::vector<int> toKeys(keysType&, const std::vector<std::string>&);
    void printAnswerPath(keysType&, const std::vector<int>&); 
};