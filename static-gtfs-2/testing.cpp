#include <iostream>
#include "gtfs.hpp"

using namespace gtfs;
using namespace std;

int main() {
    stop ohio = getStopInfo("6769");

    cout << ohio.stop_name << endl;

}