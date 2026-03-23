#include <iostream>
#include <fstream>
#include <string>
#include "../transit-files/gtfs-realtime.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
#include <libgen.h>

/* build command
clang++ -std=c++17 decodeStop.cpp ../transit-files/gtfs-realtime.pb.cc -I/opt/homebrew/opt/protobuf@21/include -L/opt/homebrew/opt/protobuf@21/lib -lprotobuf -o decodeStop
*/
using namespace std;
using namespace transit_realtime;

int main(int argc, char* argv[]) {
    std::string exeDir = dirname(argv[0]);
    std::string outputPath = exeDir + "/downloaded_stop.pb";

    std::string cmd = "wget -q -O " + outputPath + " https://rtu.york.ca/gtfsrealtime/TripUpdates";
    system(cmd.c_str());

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <stopID>" << endl;
        return 1;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    string inputFile = exeDir + "/downloaded_stop.pb";
    string targetStopId = argv[1];

    fstream input(inputFile, ios::in | ios::binary);
    if (!input) {
        cerr << "Error: could not open " << inputFile << endl;
        return 1;
    }

    FeedMessage feed;
    if (!feed.ParseFromIstream(&input)) {
        cerr << "Error: failed to parse GTFS-realtime data" << endl;
        return 1;
    }

    FeedMessage filteredFeed;
    *filteredFeed.mutable_header() = feed.header();

    bool found = false;
    for (const auto& entity : feed.entity()) {
        bool matches = false;

        // Check trip_update stop_time_updates for matching stop_id
        if (entity.has_trip_update()) {
            for (const auto& stu : entity.trip_update().stop_time_update()) {
                if (stu.stop_id() == targetStopId) {
                    matches = true;
                    break;
                }
            }
        }

        // Check vehicle position current stop
        if (entity.has_vehicle()) {
            if (entity.vehicle().stop_id() == targetStopId) {
                matches = true;
            }
        }

        // Check alerts for matching stop
        if (entity.has_alert()) {
            for (const auto& selector : entity.alert().informed_entity()) {
                if (selector.stop_id() == targetStopId) {
                    matches = true;
                    break;
                }
            }
        }

        if (matches) {
            *filteredFeed.add_entity() = entity;
            found = true;
        }
    }

    if (!found) {
        cerr << "No entities found for stop ID: " << targetStopId << endl;
        return 1;
    }

    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;

    string output;
    google::protobuf::util::MessageToJsonString(filteredFeed, &output, options);

    cout << output << endl;

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}