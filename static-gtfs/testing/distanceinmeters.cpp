#include <cmath>

double getDistanceKM(double lat1, double lon1, double lat2, double lon2) {
    const double earth_radius = 6371.0;

    double lat1_rad = lat1 * 3.1415 / 180.0;
    double lon1_rad = lon1 * 3.1415 / 180.0;
    double lat2_rad = lat2 * 3.1415 / 180.0;
    double lon2_rad = lon2 * 3.1415 / 180.0;

    double delta_lat = lat2_rad - lat1_rad;
    double delta_lon = lon2_rad - lon1_rad;

    double a = sin(delta_lat / 2) * sin(delta_lat / 2) + 
                cos(lat1_rad) * cos(lat2_rad) *
                sin(delta_lon / 2) * sin(delta_lon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return earth_radius * c;
}
