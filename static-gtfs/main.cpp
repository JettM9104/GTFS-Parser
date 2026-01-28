#include "gtfs.hpp"
#include <thread>
#include <chrono>

using namespace gtfs;


int main(int argc, char* argv[]) {
    double lat = 43.841022;
    double lon = -79.551024;

    std::vector<stop> x = getNearestStops(lat, lon);


    for (int i = 0; i < 10; i++) {
        cout << x[i].stop_name << "\t\t\t";
        cout << std::setprecision(6) << getDistanceKM(lat, lon, x[i].stop_lat, x[i].stop_lon) * 1000 << " m" << std::endl;
    }
}