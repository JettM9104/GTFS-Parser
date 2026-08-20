#include "fast-gtfs.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;

int main() {
    fast_gtfs::bin_search::sortFile(fast_gtfs::fast_stop_path, "stop_id");
    cout << "done sorting\n";


    const std::unordered_map<string, int> nn = fast_gtfs::bin_search::generateHeaderMap(fast_gtfs::fast_stop_path);
    const std::vector<pair<string, vector<string>>> na = fast_gtfs::bin_search::createMap(fast_gtfs::fast_stop_path, "stop_id");


    while (true) {
        string a;
        cin >> a;

        auto start = std::chrono::steady_clock::now();
        gtfs::stop x = fast_gtfs::bin_search::getStopInfo(a, na, nn);
        cout << x.stop_name << endl;
        auto end = std::chrono::steady_clock::now();

        auto elapsed = end - start;
        // 4. Convert and print the duration in various units
        std::cout << "Time elapsed: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()
                  << " ms\n";

        std::cout << "Time elapsed: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
                  << " µs\n";

        // Alternative: Get fractional seconds as a double without casting types
        std::chrono::duration<double> elapsed_seconds = elapsed;
        std::cout << "Time elapsed: " << elapsed_seconds.count() << " seconds\n";
    }






    return 0;
}