#ifndef main_h
#define main_h main_h
#include "main.cpp"

// this code will get the reamaing day stops of one day and print it in a nice format
// TRIP_ID @ TIME           ROUTE_ID
// 1921543 @ 23:14:23       9899

void getDayStops() {
    time24 x(11, 35, 38);
    std::vector<tripSegment> times = getRemainingDayStops(2025, 11, 7, 9794, x);

    sortVectorByTime(times);

    for (int i = 0; i < times.size(); i++) {
        cout << times[i].trip_id << " @ " << times[i].departure_time.h << ":" << times[i].departure_time.m << ":" << times[i].departure_time.s << "   \t\t " << times[i].route_id << std::endl;
    }

    cout << verifyGTFS(2025, 11, 7);
}

void findStop1() {
    cout << "c\n";
    string x1 = "SUMMIT TRAIl / BAYVIEW AV";
    std::vector<intstr> x = searchStopFromScoreAlg1(x1);

    for (int i = 0; i < 50; i++) {
        cout << x[i].num << "\t" << x[i].str << "\t\t\t" << (abs(getScore(x[i].str) - getScore(x1))) << "\n";
    }
}

#endif