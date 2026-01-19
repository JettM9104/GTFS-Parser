#include "gtfs.hpp"
#include <thread>
#include <chrono>

using namespace gtfs;


int main(int argc, char* argv[]) {
    double lat = 43.941698;
    double lon = -79.457161;

    std::vector<stop> x = getNearestStops(lat, lon);


    for (int i = 0; i < 10; i++) {
        cout << x[i].stop_name << "\t";
        cout << std::setprecision(6) << getDistanceKM(lat, lon, x[i].stop_lat, x[i].stop_lon) * 1000 << std::endl;
    }
}