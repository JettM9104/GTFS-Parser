#include "gtfs.hpp"
using namespace gtfs;
int main(int argc, char* argv[]) {
    cout << "1\n";

    std::vector<stop> x = getNearestStops(43.941698, -79.457161);

    cout << "1\n";

    for (int i = 0; i < 10; i++) {
        cout << x[i].stop_name << "\n";
    }
}