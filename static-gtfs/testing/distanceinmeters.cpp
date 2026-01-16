#include <iostream>

int getDistance(double pos1_lat, double pos1_lon, double pos2_lat, double pos2_lon) {
    double delta_lat_degrees = ((pos1_lat - pos2_lat) < 0) ? ((pos1_lat - pos2_lat) * -1) : (pos1_lat - pos2_lat);
    double delta_lon_degrees = ((pos1_lon - pos2_lon) < 0) ? ((pos1_lon - pos2_lon) * -1) : (pos1_lon - pos2_lon);

    double delta_lat_km = (delta_lat_degrees / 360) * 3.1415 * 12742;
    double delta_lon_km = (delta_lon_degrees / 360) * 3.1415 * 12742
}