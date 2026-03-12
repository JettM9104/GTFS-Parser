#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include "../static-gtfs/gtfs.hpp"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: \n" << argv[0] << " <tripID> [--precision | -p <precision>]\n";
        return -1;
    }

    int precision = 6;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-p" || arg == "--precision") && i + 1 < argc) {
            precision = std::atoi(argv[i + 1]);
            ++i;
        }
    }

    std::cout << std::fixed << std::setprecision(precision);

    int tripID;
    try {
        tripID = std::stoi(argv[1]);
    } catch (std::invalid_argument& err) {
        std::cerr << "invalid tripID\n" << std::flush;
        return 1;
    } catch (std::out_of_range& err) {
        std::cerr << "invalid tripID, out of range\n" << std::flush;
        return 2;
    } catch (...) {
        std::cerr << "how did you even manage to mess up this bad\n" << std::flush;
        return -1;
    }

    std::vector<gtfs::tripSegment> tripSegments = gtfs::getAllStops(tripID);
    gtfs::trip tx = gtfs::getTripInfo(tripID);
    gtfs::busLine bx = gtfs::getRouteInfo(tx.route_id);
    std::vector<gtfs::shape> tsx = gtfs::getShapeInfo(tx.shape_id);
    
    std::vector<gtfs::stop> stops;

    for (gtfs::tripSegment& x : tripSegments) {
        stops.push_back(gtfs::getStopInfo(x.stop_id, gtfs::ident));
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
        gtfs::tripSegment y = tripSegments[i];
        std::cout << "\t\t{ \"lat\": " << x.stop_lat << 
                ", \"lng\": " << x.stop_lon << 
                ", \"code\": "<< x.stop_code << 
                ", \"id\": " << x.stop_id << 
                ", \"time\": \"" << y.arrival_time.leadingRoundedTime() << "\"" <<
                (i == (length - 1) ? " }\n" : " },\n");
    } 

    
    cout << "\t],\n";

    int tx_length = tsx.size();

    std::cout << "\t\"pos_markers\": [\n";
    for (int i = 0; i < tx_length; i++) {
        gtfs::shape x = tsx[i];
        std::cout << "\t\t{ \"lat\": " << x.shape_pt_lat << 
                ", \"lng\": " << x.shape_pt_lon << 
                (i == (tx_length - 1) ? " }\n" : " },\n");
    } 
    
    cout << "\t]\n}\n";
    

    

}