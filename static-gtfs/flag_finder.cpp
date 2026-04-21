#include "gtfs.hpp"
#include <iostream>
#include <string>
#include <vector>

enum output_type {raw, json};

using std::cout, std::cerr, std::string;

void printUsage(string argv0) {
    cerr << "Usage: \n" << argv0 << " getdepartures [-o | --output {raw | json}]" << std::endl 
        << argv0 << " {-h | --help}" << std::endl << std::flush;
}

int main(int argc, char* argv[]) {
    output_type ot = raw;

    if (argc == 1) {
        printUsage(argv[0]);
        return 0;
    }

    for (int i = 0; i < argc; i++) {
        if (static_cast<string>(argv[i]) == "-o" || static_cast<string>(argv[i]) == "--output") {
            if ((i + 1) < argc) { 
                if (static_cast<string>(argv[i+1]) == "json") {
                    ot = json;
                } else if (static_cast<string>(argv[i+1]) == "raw") {
                    ot = raw;
                } else {
                    cerr << "Unknown usage of -o or --output flag" << std::endl;
                    printUsage(argv[0]);
                    return -1;
                }
            } else {
                printUsage(argv[0]);
                return -1;
            }
        } else if (static_cast<string>(argv[i]) == "-h" || static_cast<string>(argv[i]) == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (static_cast<string>(argv[i]) == "") {}
        
        
        else if (argv[i][0] == '-') {
            cerr << "Unknown flag " << argv[i] << std::endl;
            printUsage(argv[0]);
            return -1;
        }
    }

    cout << "\n\n\n\n";

    cout << ot << endl;

    return 0;
}