#include <iostream>
#include "gtfs.hpp"

using namespace std;


int main(int argc, char* argv[]) {
    if (argc == 1) {
        return 0;
    } else {
        string argv1 = argv[1];
        if (argv1 == "getstopinfo") {
            if (argc >= 4) {
                string argv2 = argv[2];
                if (argv2 == "ident") {
                    stop x = getStopInfo(std::stoi(argv[3]), ident);
                    x.printInfo();
                } else if (argv2 == "code") {
                    stop x = getStopInfo(std::stoi(argv[3]), code);
                    x.printInfo();
                }
            } else {
                stop x = getStopInfo(std::stoi(argv[2]), ident);
                x.printInfo();
            }

        }

    }
    
    return 0;
}