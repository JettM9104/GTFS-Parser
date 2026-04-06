#include "../static-gtfs/gtfs.hpp"

#include <iostream>
using namespace gtfs;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <route_id> <year> <month> <day>\n";
        return -1;
    }
    string trip_id = argv[1];

    vector<trip> x = getAllTrips(trip_id);

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

        if (service_y.schedule.start_date > getToday() || service_y.schedule.end_date < getToday()) {
            remove = true;
        }

        switch (convertDateToWeek(stoi(argv[2]), stoi(argv[3]), stoi(argv[4]))) {
            case mon: if (!service_y.schedule.monday) remove = true; break;
            case tue: if (!service_y.schedule.tuesday) remove = true; break;
            case wed: if (!service_y.schedule.wednesday) remove = true; break;
            case thu: if (!service_y.schedule.thursday) remove = true; break;
            case fri: if (!service_y.schedule.friday) remove = true; break;
            case sat: if (!service_y.schedule.saturday) remove = true; break;
            case sun: if (!service_y.schedule.sunday) remove = true; break;
        }

        for (gtfs::calendar_date special_day : service_y.exceptions) {
            if (special_day.date == getToday()) {
                if (special_day.exception_type == gtfs::calendar_date::removed)  remove = true;
                if (special_day.exception_type == gtfs::calendar_date::added) remove = false;
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
    cout << "{\n\t\"query_route_id\": \"" << argv[1] << "\",\n\t\"trips\": [\n";
    for (int i = 0; i < x.size(); i++) {
        trip x_a = x[i];
        cout << "\t\t\"" << x_a.trip_id << ((i == (x.size() - 1) ) ? "\"" : "\",") << endl;
    }
    cout << "\t]\n}\n";

    return 0;
}