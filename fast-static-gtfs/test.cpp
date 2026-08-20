#include "fast-gtfs.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;

int main() {
    auto start = std::chrono::steady_clock::now();
    fast_gtfs::bin_search::sortFile(fast_gtfs::fast_stop_path, "stop_id");
    cout << "done sorting\n";

    for (int i = 1000; i < 9999; i++) {
        gtfs::stop x = fast_gtfs::bin_search::getStopInfo(to_string(i));
        cout << x.stop_id << endl;
    }


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

    return 0;
}