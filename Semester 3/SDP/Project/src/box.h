#include "utilities.h"

class Box {
    Utility::keysType keys;
    static const std::string fileName;

    struct BoxInfo {
        int number;
        BoxInfo* parBox;

        std::vector<int> objects;
        std::vector<BoxInfo*> insideBoxes;
    
        void printBox(Utility::revKeysType&);

        BoxInfo();
        BoxInfo(const BoxInfo&);
        BoxInfo& operator=(const BoxInfo&);
        ~BoxInfo();
    };

    void readFile();
    bool recursiveMinimization(BoxInfo* curr);

public:
    size_t boxNumber;
    BoxInfo* root;
   
    void minimizeBoxes();
    void printAnswer();

    Box();
    Box(const Box&);
    Box& operator=(const Box&);
    ~Box();
};
