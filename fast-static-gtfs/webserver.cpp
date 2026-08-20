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


using std::cout;

void getTrip(const string& trip_id, const int& precision = 6) {


    std::cout << std::fixed << std::setprecision(precision);


    std::vector<gtfs::trip_segment> tripSegments = gtfs::getAllStops(trip_id);

    gtfs::trip tx = gtfs::getTripInfo(trip_id);

    gtfs::route bx = gtfs::getRouteInfo(tx.route_id);

    std::vector<gtfs::shape> tsx = gtfs::getShapeInfo(tx.shape_id);

    std::vector<gtfs::stop> stops;

    for (gtfs::trip_segment& x : tripSegments) {
        stops.push_back(gtfs::getStopInfo(x.stop.stop_id));
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
std::string getGreeting() {
    return "Hello from the server!";
}

int addNumbers(int a, int b) {
    return a + b;
}

std::string getStatus() {
    return "Server is running fine.";
}

int main() {
    Server svr;

    // GET /hello -> calls getGreeting()
    svr.Get("/hello", [](const Request& req, Response& res) {
        res.set_content(getGreeting(), "text/plain");
    });

    // GET /status -> calls getStatus()
    svr.Get("/status", [](const Request& req, Response& res) {
        res.set_content(getStatus(), "text/plain");
    });

    // GET /add?a=3&b=5 -> calls addNumbers(a, b), returns as string
    svr.Get("/add", [](const Request& req, Response& res) {
        int a = req.has_param("a") ? std::stoi(req.get_param_value("a")) : 0;
        int b = req.has_param("b") ? std::stoi(req.get_param_value("b")) : 0;
        int result = addNumbers(a, b);
        res.set_content(std::to_string(result), "text/plain");
    });

    std::cout << "Server listening on http://0.0.0.0:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}