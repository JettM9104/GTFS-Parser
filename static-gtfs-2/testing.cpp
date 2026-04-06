#include <iostream>
#include "gtfs.hpp"

using namespace gtfs;
using namespace std;

int main() {
    std::vector<trip_segment> a = getAllStops("1971073");

    for (trip_segment b : a) {
        cout << b.stop.arrival_time.roundedTime() << endl;
    }
}