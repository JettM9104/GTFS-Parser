#include "../static-gtfs/gtfs.hpp"

#include <iostream>
using namespace gtfs;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <tripID> <year> <month> <day>\n";
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

    vector<service> y;

    for (trip& x_trip : x) {
        y.push_back(getServiceInfo(x_trip.service_id));
    }

    int i = 0;
    while (i < y.size()) {
        service& service_y = y[i];
        bool remove = false;

        if (service_y.start_date > getToday() || service_y.end_date < getToday()) {
            remove = true;
        }

        switch (convertDateToWeek(stoi(argv[2]), stoi(argv[3]), stoi(argv[4]))) {
            case mon: if (!service_y.mon) remove = true; break;
            case tue: if (!service_y.tue) remove = true; break;
            case wed: if (!service_y.wed) remove = true; break;
            case thu: if (!service_y.thu) remove = true; break;
            case fri: if (!service_y.fri) remove = true; break;
            case sat: if (!service_y.sat) remove = true; break;
            case sun: if (!service_y.sun) remove = true; break;
        }

        for (pair<calendarDate, exceptionType>& special_day : service_y.special_dates) {
            if (special_day.first == getToday()) {
                if (special_day.second == removal)  remove = true;
                if (special_day.second == addition) remove = false;
            }
        }

        if (remove) {
            x.erase(x.begin() + i);
            y.erase(y.begin() + i);
            // don't increment i, the next element has shifted into position i
        } else {
            i++;
        }
    }
    cout << "{\n\t\"query_route_id\": " << argv[1] << ",\n\t\"trips\": [\n";
    for (int i = 0; i < x.size(); i++) {
        trip x_a = x[i];
        cout << "\t\t" << x_a.trip_id << ((i == (x.size() - 1) ) ? "" : ",") << endl;
    }
    cout << "\t]\n}\n";

    return 0;
}