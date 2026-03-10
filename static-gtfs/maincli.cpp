#include <iostream>
#include <vector>
#include <ctime>
#include "gtfs.hpp"

using namespace std;

void printUsage(string name) {
    cout << name << "\n\n" << endl;
}



int main(int argc, char* argv[]) {
    if (argc <= 0) {
        return -1;
    } else if (argc < 2) {
        printUsage(static_cast<string>(argv[0]));
        return 1;
    } else {
        vector<string> argstring;
        for (int i = 0; i < argc; i++) {
            argstring.push_back(static_cast<string>(argv[i]));
        }

        if (argstring[1] == "--help" || argstring[1] == "-help" || argstring[1] == "help") {
            printUsage(argstring[0]);
            return 0;
        } else if (argstring[1] == "getdepartures") {
            if (argc == 3) {
                time_t rn = time(0);
                tm* ltm = localtime(&rn);

                int year = 1900 + ltm->tm_year;
                int month = 1 + ltm->tm_mon;
                int day = ltm->tm_mday;

                vector<gtfs::tripSegment> dep = gtfs::getDayTimesAtStop(year, month, day, stoi(argstring[2]));

                for (gtfs::tripSegment sdep : dep) {
                    cout << sdep.trip_id << "->" << sdep.route_id << "\t@ " << sdep.departure_time.roundedTime() << endl;
                }
                return 0;
            } else if (argc == 6) {
                int year = stoi(argstring[3]);
                int month = stoi(argstring[4]);
                int day = stoi(argstring[5]);

                vector<gtfs::tripSegment> dep = gtfs::getDayTimesAtStop(year, month, day, stoi(argstring[2]));

                for (gtfs::tripSegment sdep : dep) {
                    cout << sdep.trip_id << "->" << sdep.route_id << "\t@ " << sdep.departure_time.roundedTime() << endl;
                }

                return 0;
            } else {
                printUsage(argstring[0]);
                return 1;
            }
        } else if (argstring[1] == "getneareststops") {
            if (argc == 4) {
                double lat;
                double lon;

                lat = stod(argstring[2]);
                lon = stod(argstring[3]);

                vector<gtfs::stop> sto = gtfs::getNearestStops(lat, lon);
                

                for (gtfs::stop ssto : sto) {
                    double distKM =  gtfs::getDistanceKM(lat, lon, ssto.stop_lat, ssto.stop_lon);
                    cout << ssto.stop_id << "\t" << ssto.stop_lat << ", " << ssto.stop_lon << "\t";
                    if (distKM >= 1) {
                        cout << distKM << " km\n";
                    } else {
                        cout << distKM * 1000 << " m\n";
                    }

                }
                return 0;
            } else if (argc == 5) {
                double lat;
                double lon;
                int n;

                lat = stod(argstring[2]);
                lon = stod(argstring[3]);
                n = stoi(argstring[4]);

                vector<gtfs::stop> sto = gtfs::getNearestStops(lat, lon);

                for (int i = 0; i < n; i++) {
                    gtfs::stop ssto = sto[i];
                    double distKM =  gtfs::getDistanceKM(lat, lon, ssto.stop_lat, ssto.stop_lon);
                    cout << ssto.stop_id << " - "<< ssto.stop_name << "\t" << ssto.stop_lat << ", " << ssto.stop_lon << "\t";

                    if (distKM >= 1) {
                        cout << distKM << " km\n";
                    } else {
                        cout << distKM * 1000 << " m\n";
                    }
                }
                return 0;
            } else {
                printUsage(argstring[0]);
                return 1;
            }
        }
    }

    return 0;
}