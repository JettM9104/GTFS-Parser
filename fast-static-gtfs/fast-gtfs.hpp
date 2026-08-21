// this file is for experimetial features, that will improve the speed of the
//      quering hopefully by a lot. Implementations like hashing, binary 
//      search lexographically, are all here. Stil a work in progress.
//
//      When utilizing this header file try to avoid using the line 
//      "using namespace fast_gtfs" while also having "using namespace gtfs"
//      because function names may collide.

#ifndef FAST_GTFS_HPP
#define FAST_GTFS_HPP

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../static-gtfs/gtfs.hpp"
#include "fast-config.hpp"

using std::cout, std::string, std::ifstream, std::ofstream, std::cerr, std::to_string, std::endl, std::vector, std::pair;

namespace fast_gtfs {

namespace bin_search {

inline void sortFile(const string& path, const string& keyColumn, const string& outputpath) { // e.g. for stops.txt's stop_id or trips.txt's trip_id
    string header;
    std::vector<std::pair<string, string>> keyedLines;
    {
        ifstream in(path);
        std::getline(in, header);
        auto refs = gtfs::createMapFromVector(gtfs::parseDataCSV(header));
        int keyIdx = refs[keyColumn];

        string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            string key = gtfs::parseDataCSV(line)[keyIdx];
            keyedLines.emplace_back(std::move(key), std::move(line));
        }
    }

    std::sort(keyedLines.begin(), keyedLines.end(),
        [](const std::pair<string, string>& a,
           const std::pair<string, string>& b) { return a.first < b.first; });

    ofstream out(outputpath, std::ios::trunc);
    out << header << '\n';
    for (const auto& kl : keyedLines) out << kl.second << '\n';
}
inline vector<pair<string, vector<string>>> createMap(const string& path, const string& key) {
    ifstream stopFile(path);
    string header;
    std::getline(stopFile, header);
    auto refs = gtfs::createMapFromVector(gtfs::parseDataCSV(header));

    vector<pair<string, vector<string>>> lines;

    string currentLine;

    while (getline(stopFile, currentLine)) {
        if (currentLine.empty()) continue;
        vector<string> parsedCurrentLine = gtfs::parseDataCSV(currentLine);
        lines.emplace_back(parsedCurrentLine[refs[key]], parsedCurrentLine);
    }

    stopFile.close();
    return lines;
}
inline std::unordered_map<string, int> generateHeaderMap(const string& path) {
    ifstream stopFile(path);
    string header;
    std::getline(stopFile, header);
    auto refs = gtfs::createMapFromVector(gtfs::parseDataCSV(header));

    stopFile.close();

    return refs;
}
inline gtfs::stop getStopInfo(const string& stop_id, const vector<pair<string, vector<string>>>& lines, const std::unordered_map<string, int>& refs) { // binary search algorithm (MUST BE SORTED LEXOGRAPHICALLT first)
    gtfs::stop output;
    output.stop_id = "-1";


    auto stop_index = std::lower_bound(lines.begin(), lines.end(), stop_id,
        [](const std::pair<std::string, std::vector<string>>& element, const std::string& key) {
                return element.first < key;
        });

    if (stop_index == lines.end() || stop_index->first != stop_id) return output;

    // required fields
    output.stop_id = stop_id;


    // optional/conditionally required/conditionally forbidden fields
    { auto find = refs.find("stop_code");
    if (find != refs.end()) output.stop_code = stop_index->second[find->second]; }

    { auto find = refs.find("stop_name");
    if (find != refs.end()) output.stop_name = stop_index->second[find->second]; }

    { auto find = refs.find("tts_stop_name");
    if (find != refs.end()) output.tts_stop_name = stop_index->second[find->second]; }

    { auto find = refs.find("stop_desc");
    if (find != refs.end()) output.stop_desc = stop_index->second[find->second]; }

    { auto find = refs.find("stop_lat");
    if (find != refs.end()) output.stop_lat = gtfs::to_double(stop_index->second[find->second]); }

    { auto find = refs.find("stop_lon");
    if (find != refs.end()) output.stop_lon = gtfs::to_double(stop_index->second[find->second]); }

    { auto find = refs.find("zone_id");
    if (find != refs.end()) output.zone_id = stop_index->second[find->second]; }

    { auto find = refs.find("stop_url");
    if (find != refs.end()) output.stop_url = stop_index->second[find->second]; }

    { auto find = refs.find("location_type");
    if (find != refs.end()) output.location_type = static_cast<gtfs::stop::location>(gtfs::to_integer(stop_index->second[find->second])); }

    { auto find = refs.find("parent_station");
    if (find != refs.end()) output.parent_station = stop_index->second[find->second]; }

    { auto find = refs.find("stop_timezone");
    if (find != refs.end()) output.stop_timezone = stop_index->second[find->second]; }

    { auto find = refs.find("wheelchair_boarding");
    if (find != refs.end()) output.wheelchair_boarding = static_cast<gtfs::stop::wheelchair>(gtfs::to_integer(stop_index->second[find->second])); }

    { auto find = refs.find("level_id");
    if (find != refs.end()) output.level_id = stop_index->second[find->second]; }

    { auto find = refs.find("platform_code");
    if (find != refs.end()) output.platform_code = stop_index->second[find->second]; }

    { auto find = refs.find("stop_access");
    if (find != refs.end()) output.stop_access = static_cast<gtfs::stop::access>(gtfs::to_integer(stop_index->second[find->second])); }


    return output;
}
inline gtfs::trip getTripInfo(const string& trip_id, const vector<pair<string, vector<string>>>& lines, const std::unordered_map<string, int>& refs) {
    gtfs::trip output;
    output.trip_id = "-1";


    auto trip_index = std::lower_bound(lines.begin(), lines.end(), trip_id,
        [](const std::pair<std::string, std::vector<string>>& element, const std::string& key) {
                return element.first < key;
        });

    if (trip_index == lines.end() || trip_index->first != trip_id) return output;

    output.route_id = trip_index->second[refs.at("route_id")];
    output.service_id = trip_index->second[refs.at("service_id")];
    output.trip_id = trip_id;

    // optional/conditionally required/conditionally forbidden fields
    { auto find = refs.find("trip_headsign");
        if (find != refs.end()) output.trip_headsign = trip_index->second[find->second]; }

    { auto find = refs.find("trip_short_name");
        if (find != refs.end()) output.trip_short_name = trip_index->second[find->second]; }

    { auto find = refs.find("block_id");
        if (find != refs.end()) output.block_id = trip_index->second[find->second]; }

    { auto find = refs.find("shape_id");
        if (find != refs.end()) output.shape_id = trip_index->second[find->second]; }

    // bool field
    { auto find = refs.find("direction_id");
        if (find != refs.end()) output.direction_id = static_cast<bool>(stoi(trip_index->second[find->second])); }

    // allowable fields
    { auto find = refs.find("wheelchair_accessible");
        if (find != refs.end()) output.wheelchair_accessible = static_cast<gtfs::trip::allowable>(stoi(trip_index->second[find->second])); }

    { auto find = refs.find("bikes_allowed");
        if (find != refs.end()) output.bikes_allowed = static_cast<gtfs::trip::allowable>(stoi(trip_index->second[find->second])); }

    { auto find = refs.find("cars_allowed");
        if (find != refs.end()) output.cars_allowed = static_cast<gtfs::trip::allowable>(stoi(trip_index->second[find->second])); }

    return output;
}
inline vector<gtfs::trip_segment>(const string& trip_id, const vector<string>& lines, const std::unordered_map<string, int>& refs) {
    vector<gtfs::trip_segment> output;

    

    return output;
}

}

};

#endif