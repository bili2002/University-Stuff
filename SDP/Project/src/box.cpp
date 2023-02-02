#include <algorithm>
#include <stack>
#include "box.h"

const std::string Box::fileName = "boxesList.txt";

void Box::readFile() {
    std::ifstream is(fileName);
    if (!is.is_open()) {
        throw std::runtime_error("Couldn't open file.");
    }

    is>>boxNumber;

    std::unordered_map<int, BoxInfo*> boxPtrs;
    std::unordered_map<int, BoxInfo*> parents;

    for (size_t i=0; i<boxNumber; i++) {
        std::string boxName;
        int objNum, boxesNum;

        is>>boxName;
        Utility::addKey(keys, boxName);

        int boxKey = keys[boxName];
        auto box = new BoxInfo();
        box->number = boxKey;
        boxPtrs[boxKey] = box;

        is>>objNum;
        while (objNum-- > 0) {
            std::string objName;
            is>>objName;

            Utility::addKey(keys, objName);
            box->objects.push_back(keys[objName]);
        }

        is>>boxesNum;
        while (boxesNum-- > 0) {
            std::string insideBoxName;
            is>>insideBoxName;

            Utility::addKey(keys, insideBoxName);

            int insideBoxKey = keys[insideBoxName];
            parents[insideBoxKey] = box;
        }
    }


    for (auto& currBoxItr : boxPtrs) {
        int key = currBoxItr.first;
        
        BoxInfo* childBox = currBoxItr.second;
        BoxInfo* parBox;

        if (parents.find(key) == parents.end()) {
            parBox = root;
        } else {  
            parBox = parents[key];
        }    

        parBox->insideBoxes.push_back(childBox);
        childBox->parBox = parBox;
    }

    is.close();
}

bool Box::recursiveMinimization(BoxInfo* curr) {  
    for (size_t i=0; i<curr->insideBoxes.size(); i++) {
        BoxInfo* child = curr->insideBoxes[i];

        if (recursiveMinimization(child)) {
            std::swap(curr->insideBoxes[i], curr->insideBoxes.back());

            if (curr->insideBoxes.back() != nullptr) {
                delete curr->insideBoxes.back();
            }
            curr->insideBoxes.pop_back();
            boxNumber--;
        }
    }
    
    if (curr->objects.empty() && curr->insideBoxes.empty()) {
        return true;
    }

    if (curr->objects.empty() && curr->insideBoxes.size() == 1) {
        curr->parBox->insideBoxes.push_back(curr->insideBoxes[0]); 
        curr->insideBoxes.pop_back();

        return true;
    }

    return false;
}

void Box::minimizeBoxes() {
    for (BoxInfo* curr : root->insideBoxes) {
        recursiveMinimization(curr);
    }   
}

void Box::printAnswer() {
    std::cout<<boxNumber<<std::endl;
    
    Utility::revKeysType revKeys;
    Utility::createRevKeys(keys, revKeys);

    std::stack<BoxInfo*> boxesStack;
    boxesStack.push(root);

    while (!boxesStack.empty()) {
        BoxInfo* curr = boxesStack.top();
        boxesStack.pop();

        for (BoxInfo* child : curr->insideBoxes) {
            boxesStack.push(child);
        } 

        if (curr != root) {
            curr->printBox(revKeys);
        }
    } 
}

Box::Box() {
    root = new BoxInfo();

    readFile(); 
}

Box::Box(const Box& oth) {
    *this = oth;
}

Box& Box::operator=(const Box& oth) {
   if (this == &oth) {
        return *this;
    }

    boxNumber = oth.boxNumber;
    keys = oth.keys;
    root = oth.root;

    return *this;
}

Box::~Box() {
    delete root;
}

void Box::BoxInfo::printBox(Utility::revKeysType& revKeys) {
    std::cout<<revKeys[number]<<' ';

    std::cout<<objects.size()<<' ';
    for (int currObj : objects) {
        std::cout<<revKeys[currObj]<<' ';
    }
    
    std::cout<<insideBoxes.size()<<' ';
    for (BoxInfo* currBox : insideBoxes) {
        std::cout<<revKeys[currBox->number]<<' ';
    }

    std::cout<<'\n';
}

Box::BoxInfo::BoxInfo() {
    parBox = nullptr;
}

Box::BoxInfo::BoxInfo(const BoxInfo& oth) {
    *this = oth;
}

Box::BoxInfo& Box::BoxInfo::operator=(const BoxInfo& oth) {
    if (this == &oth) {
        return *this;
    }

    number = oth.number;
    parBox = oth.parBox;

    objects = oth.objects;

    insideBoxes.resize(oth.insideBoxes.size(), nullptr);
    for (size_t i=0; i<oth.insideBoxes.size(); i++) {
        insideBoxes[i] = new BoxInfo(*oth.insideBoxes[i]);
    }

    return *this;
}

Box::BoxInfo::~BoxInfo() {
    for (BoxInfo* curr : insideBoxes) {
        delete curr;
    }
}