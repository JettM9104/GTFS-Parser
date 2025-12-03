#include <iostream>
#include "gtfs.hpp"

using namespace std;


int main(int argc, char* argv[]) {
    if (argc == 1) {
        return 0;
    } else {
        string argv1 = argv[1];
        if (argv1 == "getstopinfo") {
            stop x = getStopInfo(std::stoi(argv[2]), ident);
            x.printInfo();
        }
    }
    
    return 0;
}