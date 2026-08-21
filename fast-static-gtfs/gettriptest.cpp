#include "httplib.h"
#include <iostream>
#include <string>
#include "../static-gtfs/gtfs.hpp"
#include "fast-gtfs.hpp"
using namespace httplib;

// --- Your C++ functions ---
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>

vector<pair<string, vector<string>>> triplines;
std::unordered_map<string, int> triprefs;

vector<pair<string, vector<string>>> stoplines;
std::unordered_map<string, int> stoprefs;

vector<pair<string, vector<string>>> stoptimesstoplines;
std::unordered_map<string, int> stoptimesstoprefs;


using std::cout;

void getTrip(const string& trip_id, const int& precision = 6) {

    std::cout << std::fixed << std::setprecision(precision);


    std::vector<gtfs::trip_segment> tripSegments = fast_gtfs::bin_search::getAllStops(trip_id, stoptimesstoplines, stoptimesstoprefs);
    gtfs::trip tx = fast_gtfs::bin_search::getTripInfo(trip_id, triplines, triprefs);
    gtfs::route bx = gtfs::getRouteInfo(tx.route_id);
    std::vector<gtfs::shape> tsx = gtfs::getShapeInfo(tx.shape_id);

    std::vector<gtfs::stop> stops;

    for (gtfs::trip_segment& x : tripSegments) {
        stops.push_back(fast_gtfs::bin_search::getStopInfo(x.stop.stop_id, stoplines, stoprefs));
    }
    int length = stops.size();

    std::cout << "{\n";
    std::cout << "\t\"total\": " << length << ",\n";
    std::cout << "\t\"trip_id\": " << tx.trip_id << ",\n";
    std::cout << "\t\"route_id\": " << tx.route_id << ",\n";
    std::cout << "\t\"route_short_name\": \"" << bx.route_short_name << "\",\n";
    std::cout << "\t\"route_long_name\": \"" << bx.route_long_name << "\",\n";
    std::cout << "\t\"route_color\": \"#" << bx.route_color << "\",\n";

    std::cout << "\t\"stops\": [\n";
    for (int i = 0; i < length; i++) {
        gtfs::stop x = stops[i];
        gtfs::trip_segment y = tripSegments[i];
        std::cout << "\t\t{ \"lat\": " << x.stop_lat <<
                ", \"lng\": " << x.stop_lon <<
                ", \"code\": \""<< x.stop_code <<
                "\", \"id\": \"" << x.stop_id <<
                "\", \"name\": \"" << x.stop_name <<
                "\", \"time\": \"" << y.stop.arrival_time.leadingRoundedTime() << "\"" <<
                ", \"stop_sequence\": " << y.stop.stop_sequence <<
                (i == (length - 1) ? " }\n" : " },\n");
    }


    cout << "\t],\n";

    int tx_length = tsx.size();

    std::cout << "\t\"pos_markers\": [\n";
    for (int i = 0; i < tx_length; i++) {
        gtfs::shape x = tsx[i];
        std::cout << "\t\t{ \"lat\": " << x.shape_pt_lat <<
                ", \"lng\": " << x.shape_pt_lon <<
                ", \"sequence\": " << x.shape_pt_sequence <<
                (i == (tx_length - 1) ? " }\n" : " },\n");
    }

    cout << "\t]\n}\n";


}


int main() {
    fast_gtfs::bin_search::sortFile(fast_config::fast_stop_path, "stop_id", fast_config::fast_stop_stop_id);
    fast_gtfs::bin_search::sortFile(fast_config::fast_stop_times_path, "trip_id", fast_config::fast_stop_times_trip_id);

    triplines = fast_gtfs::bin_search::createMap(fast_config::fast_trip_path, "trip_id");
    triprefs =  fast_gtfs::bin_search::generateHeaderMap(fast_config::fast_trip_path);

    stoplines = fast_gtfs::bin_search::createMap(fast_config::fast_stop_stop_id, "stop_id");
    stoprefs = fast_gtfs::bin_search::generateHeaderMap(fast_config::fast_stop_stop_id);

    stoptimesstoplines = fast_gtfs::bin_search::createMap(fast_config::fast_stop_times_trip_id, "trip_id");
    stoptimesstoprefs = fast_gtfs::bin_search::generateHeaderMap(fast_config::fast_stop_times_trip_id);

    cout << "done init\n";

    while (1) {
        string a;
        std::cin >> a;

        getTrip(a);
        std::cout << "done\n";
    }
    return 0;
}