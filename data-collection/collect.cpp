#include <iostream>
#include <fstream>
#include <string>
#include "../gtfs-rt/proto-conversion/transit-files/gtfs-realtime.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
#include <libgen.h>

// build command: clang++ -std=c++17 -O3 collect.cpp ../gtfs-rt/proto-conversion/transit-files/gtfs-realtime.pb.cc $(pkg-config --cflags --libs protobuf) -o collect && ./collect

using namespace std;
using namespace transit_realtime;

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::string exeDir = dirname(argv[0]);
    std::string outputPath = exeDir + "/downloaded_tripUpdates.pb";

    std::string cmd = "wget -q -O '" + outputPath + "' https://rtu.york.ca/gtfsrealtime/TripUpdates";
    system(cmd.c_str());

    // Open the GTFS-realtime feed
    fstream input(outputPath, ios::in | ios::binary);
    if (!input) {
        cerr << "Error: could not open downloaded_tripUpdates.pb" << endl;
        return 1;
    }

    // Parse the binary protobuf
    FeedMessage feed;
    if (!feed.ParseFromIstream(&input)) {
        cerr << "Error: failed to parse GTFS-realtime data" << endl;
        return 1;
    }

    // Output file
    ofstream outfile("decoded_gtfs.json");
    if (!outfile) {
        cerr << "Error: could not open output file" << endl;
        return 1;
    }

    // Convert to JSON format
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_fields_with_no_presence = true;
    options.preserve_proto_field_names = true;
    string output;
    google::protobuf::util::MessageToJsonString(feed, &output, options);

    // Write to file
    outfile << output;
    outfile.close();

    cout << "GTFS data successfully written to decoded_gtfs.json" << endl;

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}