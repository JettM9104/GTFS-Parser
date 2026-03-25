#include "../static-gtfs/gtfs.hpp"

#include <iostream>
using namespace gtfs;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <tripID>\n";
        return -1;
    }
    int tID;

    try {
        tID = stoi(argv[1]);
    } catch (std::invalid_argument& err) {
        std::cerr << "invalid coordinates\n" << std::flush;
        return 1;
    } catch (std::out_of_range& err) {
        std::cerr << "invalid coordinates, out of range\n" << std::flush;
        return 2;
    } catch (...) {
        std::cerr << "...\n" << std::flush;
        return -1;
    }

    vector<trip> x = getAllTrips(tID);

    for (trip& x_trip : x) {
        x_trip = getTripInfo(x_trip.trip_id);
    }

    

    service y = getServiceInfo("152.0.1");
    cout << y.sun << y.mon << y.tue << y.wed << y.thu << y.fri << y.sat << endl;

    for (pair<calendarDate, exceptionType> x : y.special_dates) {
        x.first.printInfo();
        cout << endl;
        cout << x.second << "\n\n\n";
    }
}