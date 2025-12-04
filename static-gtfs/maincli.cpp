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

        } else if (argv1 == "searchstop") {
            string word = "";
            for (int i = 0; i < argc - 2; i++) {
                string curarg = argv[i + 2];
                word += curarg;
                word += " ";
            }
            std::vector<matchsearch> x = searchStopFromScoreAlg2(word);

            for (int i = x.size() - 1; i >= 0; i--) {
                cout << i << ": " << x[i].score << " | " << x[i].text.str << " | ";
                cout << x[i].text.num << std::endl;
            }

            cout << word;
        }

    }
    
    return 0;
}