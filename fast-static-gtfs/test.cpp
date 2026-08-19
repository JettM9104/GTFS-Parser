#include "fast-gtfs.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    fast_gtfs::bin_search::sortFile("test-data/trips.txt", "trip_id");

    cout << "done sorting\n";
}