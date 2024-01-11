#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <stdlib.h>   
#include <time.h>  


int main(int argc, char* argv[]) {
    srand (time(NULL));

    if (argc < 4) {
        std::cerr << "Wrong arguments";
        return 1;
    }

    std::string filename = argv[1];
    int n = std::atoi(argv[2]);
    int percentageOf1 = std::atoi(argv[3]);

    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr<<"File didn't open"<<std::endl;
        return 1;
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            file<<(rand() % 100 < percentageOf1 ? 1 : 0);
        }
        file<<'\n';
    }

    return 0;
}

