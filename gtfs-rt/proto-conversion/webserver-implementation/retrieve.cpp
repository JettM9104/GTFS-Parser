#include <cstdio>
#include <string>
#include <libgen.h>

int main(int argc, char* argv[]) {
    std::string exeDir = dirname(argv[0]);
    std::string outputPath = exeDir + "/downloaded_file.pb";

    std::string cmd = "wget -q -O " + outputPath + " https://rtu.york.ca/gtfsrealtime/VehiclePositions";
    system(cmd.c_str());

    return 0;
}