#include "fast-gtfs.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    fast_gtfs::bin_search::sortFile("test-data/trips.txt", "trip_id");

    int x = 5;

    vector<int> test;

    for (int i = 0; i < 5; i++) {
        test.push_back(i);
    }

    for (int k : test) {
        cout << k << endl;
    }
}