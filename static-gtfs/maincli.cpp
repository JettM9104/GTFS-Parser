#include <iostream>
#include "gtfs.hpp"

using namespace std;
using namespace gtfs;

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

            for (int i = 25; i >= 0; i--) {
                cout << i << ": " << x[i].score << " | " << x[i].text.str << " | ";
                cout << x[i].text.num << std::endl;
            }

            cout << word;
        } else if (argv1 == "getdepartures") {
            int argv2 = stoi(argv[2]);
            int argv3 = stoi(argv[3]);
            int argv4 = stoi(argv[4]);
            short argv5 = stoi(argv[5]);

            vector<tripSegment> x = getDayTimesAtStop(argv2, argv3, argv4, argv5);


            sortVectorByTime(x);

            for (int i = 0; i < x.size(); i++) {
                cout << x[i].trip_id << " @ " << x[i].departure_time.h << ":" << x[i].departure_time.m << ":" << x[i].departure_time.s << "   \t\t " << x[i].route_id << std::endl;
            }

            int v = verifyGTFS(argv2, argv3, argv4);

            if (v == -1) {
                cout << "-1, this feed has expired\n";
            } else if (v == 0) {
                cout << "0, this feed has not yet been put info place\n";
            } else {
                cout << "1, this feed is valid\n";
            }
        }

    }
    
    return 0;
}