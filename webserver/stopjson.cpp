#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include "../static-gtfs/gtfs.hpp"
#include <ctime>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: \n" << argv[0] << " <stopID> [year, month, day]";
        return -1;
    }

    gtfs::stop st;

    try {
        st = gtfs::getStopInfo(stoi(argv[1]), gtfs::ident);
    } catch (std::invalid_argument& err) {
        std::cerr << "invalid stopID\n" << std::flush;
        return 1;
    } catch (std::out_of_range& err) {
        std::cerr << "invalid stopID, out of range\n" << std::flush;
        return 2;
    } catch (...) {
        std::cerr << "how\n" << std::flush;
        return -1;
    }
    std::time_t rn = std::time(0);
    std::tm* ltm = std::localtime(&rn);

    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;

    std::vector<gtfs::tripSegment> ts = gtfs::getDayTimesAtStop(year, month, day, st.stop_id);

    std::cout << "{\n\t\"stop_id\": " << argv[1] << ",\n"
                << "\t\"stop_code\": " << st.stop_code << ",\n"
                << "\t\"stop_name\": \"" << st.stop_name << "\",\n"
                << "\t\"lat\": " << st.stop_lat << ",\n"
                << "\t\"lon\": " << st.stop_lon << ",\n";
    
    std::cout << "\t\"departures\": [\n";

    const int tsLen = ts.size();
    for (int i = 0; i < tsLen; i++) {
        gtfs::tripSegment x = ts[i];
        cout << "\t\t{ \"route_id\": " << x.route_id << 
                ", \"arrival_time\": \"" << x.arrival_time.leadingRoundedTime() << 
                "\", \"trip_id\": " << x.trip_id << 
                (i == (tsLen-1) ? " } \n" : " }, \n");
    }

    cout << "\t]\n}\n";

}