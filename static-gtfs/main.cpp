#include "gtfs.hpp"
using namespace gtfs;
int main(int argc, char* argv[]) {

    std::vector<stop> x = getAllStops(1904660);

    for (stop a : x) {
        cout << a.stop_id << " ";
    }
}