#include "gtfs.hpp"
#include <thread>
#include <chrono>

using namespace gtfs;
using namespace std;


int main(int argc, char* argv[]) {
    gtfs::feedStatus x = verifyGTFS(2028, 3, 1);

    cout << x;
}