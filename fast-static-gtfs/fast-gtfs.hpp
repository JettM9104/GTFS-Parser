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

using std::cout, std::string, std::ifstream, std::ofstream, std::cerr, std::to_string, std::endl;

namespace fast_gtfs {
//

namespace bin_search {

void sortFile(const string& path, const string& keyColumn) {
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

    ofstream out(path, std::ios::trunc);
    out << header << '\n';
    for (const auto& kl : keyedLines) out << kl.second << '\n';
}

}

};

#endif