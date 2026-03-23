#include "gtfs.hpp"
#include <thread>
#include <chrono>

using namespace gtfs;
using namespace std;


int main(int argc, char* argv[]) {
    std::vector<gtfs::trip> x = getAllTrips(9102);

    for (trip y : x) {
        cout << y.trip_id << " ";
    }

    x[0] = gtfs::getTripInfo(x[0].trip_id);

    cout << x[0].service_id << endl;

    gtfs::
}