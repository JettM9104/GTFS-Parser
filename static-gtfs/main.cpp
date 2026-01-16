#include "gtfs.hpp"
using namespace gtfs;
int main(int argc, char* argv[]) {
    double lat = 43.941698;
    double lon = -79.457161;

    std::vector<stop> x = getNearestStops(lat, lon);


    for (int i = 0; i < 10; i++) {
        cout << x[i].stop_name << "\t";
        cout << std::setprecision(17) << getDistanceKM(lat, lon, x[i].stop_lat, x[i].stop_lon) << std::endl;
    }
}