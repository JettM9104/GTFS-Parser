#include "fast-gtfs.hpp"
#include <iostream>
#include <fstream>

int main() {

    fast_gtfs::bin_search::sortFile("test-data/trips.txt", "trip_id");
}