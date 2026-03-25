#include "../static-gtfs/gtfs.hpp"

#include <iostream>

using namespace std;
using namespace gtfs;

int main() {
    // std::vector<trip> x = getAllTrips(9102); 
    // for (trip& a : x) {
    //     a = getTripInfo(a.trip_id);
    // }
    // for (trip& a : x) {
    //     cout << a.trip_id << " " << a.service_id << endl;
    // }
    // service y = getServiceInfo(x[0].service_id); 



    service y = getServiceInfo("152.0.1");
    cout << y.sun << y.mon << y.tue << y.wed << y.thu << y.fri << y.sat << endl;

    for (pair<calendarDate, exceptionType> x : y.special_dates) {
        x.first.printInfo();
        cout << endl;
        cout << x.second << "\n\n\n";
    }
}