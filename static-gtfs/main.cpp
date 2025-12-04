#include "gtfs.hpp"

int main(int argc, char* argv[]) {
    time24 x(0,0,0);
    std::vector<tripSegment> times = getRemainingDayStops(2025, 11, 7, 6281, x);

    sortVectorByTime(times);

    for (int i = 0; i < times.size(); i++) {
        cout << times[i].trip_id << " @ " << times[i].departure_time.h << ":" << times[i].departure_time.m << ":" << times[i].departure_time.s << "   \t\t " << times[i].route_id << std::endl;
    }

    cout << verifyGTFS(2025, 11, 7);
}