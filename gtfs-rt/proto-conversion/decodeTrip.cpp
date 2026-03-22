#include <iostream>
#include <fstream>
#include <string>
#include "gtfs-realtime.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

/* build commadn
clang++ -std=c++17 decodeTrip.cpp gtfs-realtime.pb.cc -I/opt/homebrew/opt/protobuf@21/include -L/opt/homebrew/opt/protobuf@21/lib -lprotobuf -o decodeTrip
*/
using namespace std;
using namespace transit_realtime;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input.pb> <tripID>" << endl;
        return 1;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    string inputFile = argv[1];
    string targetTripId = argv[2];

    // Open the GTFS-realtime feed
    fstream input(inputFile, ios::in | ios::binary);
    if (!input) {
        cerr << "Error: could not open " << inputFile << endl;
        return 1;
    }

    // Parse the binary protobuf
    FeedMessage feed;
    if (!feed.ParseFromIstream(&input)) {
        cerr << "Error: failed to parse GTFS-realtime data" << endl;
        return 1;
    }

    // Filter entities by trip ID
    FeedMessage filteredFeed;
    *filteredFeed.mutable_header() = feed.header();

    bool found = false;
    for (const auto& entity : feed.entity()) {
        bool matches = false;

        if (entity.has_trip_update() &&
            entity.trip_update().has_trip() &&
            entity.trip_update().trip().trip_id() == targetTripId) {
            matches = true;
        }

        if (entity.has_vehicle() &&
            entity.vehicle().has_trip() &&
            entity.vehicle().trip().trip_id() == targetTripId) {
            matches = true;
        }

        if (entity.has_alert()) {
            for (const auto& selector : entity.alert().informed_entity()) {
                if (selector.trip().trip_id() == targetTripId) {
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
        cerr << "No entities found for trip ID: " << targetTripId << endl;
        return 1;
    }

    // Convert filtered feed to JSON
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