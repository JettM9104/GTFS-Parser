#include "gtfs.hpp"

int main(int argc, char* argv[]) {

    stop nice = getStopInfo(7514, code);
    nice.printInfo();

    cout << "\n\n\n";


    time24 x(11, 35, 38);
    std::vector<tripSegment> times = getDayTimesAtStop(2025, 11, 12, 7514);

    
    sortVectorByTime(times);

    for (int i = 0; i < times.size(); i++) {
        cout << times[i].trip_id << " @ " << times[i].departure_time.h << ":" << times[i].departure_time.m << ":" << times[i].departure_time.s << "   \t\t " << times[i].route_id << std::endl;
    }

    cout << verifyGTFS(2025, 11, 7);
}
