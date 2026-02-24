#include "gtfs.hpp"
#include <thread>
#include <chrono>

using namespace gtfs;
using namespace std;


int main(int argc, char* argv[]) {
    double lat = 43.941701;
    double lon = -79.456205;

    vector<tripSegment> x = getDayTimesAtStop(2026, 02, 24, 6769);

    for (tripSegment& i : x) {
        i.printInfo();
        cout << "\n\n";
    }

    
}