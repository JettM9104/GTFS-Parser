#pragma once
#include <string>
#include <fstream>
#include <libgen.h>
#include "../static-gtfs/gtfs.hpp"

// Resolves the path to config.json sitting next to the binary.
static std::string _gtfsCfgFilePath(const char* argv0) {
    char resolved[PATH_MAX];
    char input[PATH_MAX];
    strncpy(input, argv0, PATH_MAX - 1);
    input[PATH_MAX - 1] = '\0';
    if (realpath(argv0, resolved)) {
        char copy[PATH_MAX];
        strncpy(copy, resolved, PATH_MAX - 1);
        copy[PATH_MAX - 1] = '\0';
        return std::string(dirname(copy)) + "/config.json";
    }
    return std::string(dirname(input)) + "/config.json";
}

// Reads data_path from config.json next to the binary and applies it to all
// gtfs:: path variables.  No-op if the file is absent or has no data_path key.
static void loadGtfsConfig(const char* argv0) {
    std::string cfgFile = _gtfsCfgFilePath(argv0);
    std::ifstream f(cfgFile);
    if (!f.is_open()) return;
    std::string content, line;
    while (std::getline(f, line)) content += line;
    auto pos = content.find("\"data_path\"");
    if (pos == std::string::npos) return;
    pos = content.find('"', pos + 11);
    if (pos == std::string::npos) return;
    ++pos;
    auto end = content.find('"', pos);
    if (end == std::string::npos) return;
    std::string dataPath = content.substr(pos, end - pos);
    if (dataPath.empty()) return;
    if (dataPath.back() != '/') dataPath += '/';
    gtfs::root              = dataPath;
    gtfs::stopPath          = dataPath + "stops.txt";
    gtfs::routePath         = dataPath + "routes.txt";
    gtfs::tripsPath         = dataPath + "trips.txt";
    gtfs::stopTimesPath     = dataPath + "stop_times.txt";
    gtfs::tripPath          = dataPath + "trips.txt";
    gtfs::calendarPath      = dataPath + "calendar.txt";
    gtfs::calendarDatesPath = dataPath + "calendar_dates.txt";
    gtfs::agencyPath        = dataPath + "agency.txt";
    gtfs::shapePath         = dataPath + "shapes.txt";
    gtfs::feedInfoFile      = dataPath + "feed_info.txt";
}
