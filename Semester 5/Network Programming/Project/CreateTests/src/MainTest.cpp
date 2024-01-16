#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <stdlib.h>   
#include <time.h>  

const int MAXN = 1e4;

int main(int argc, char* argv[]) {
    srand (time(NULL));

    if (argc < 3) {
        std::cerr << "Wrong arguments";
        return 1;
    }

    std::string filename = argv[1];
    int n = std::atoi(argv[2]);

    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File didn't open" << std::endl;
        return 1;
    }

    for (int i=0; i<n; i++) {
        file << rand() % MAXN << ' ';
    }
    file << '\n';

    return 0;
}

