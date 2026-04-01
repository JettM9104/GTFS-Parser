#include <iostream>
#include <fstream>
#include <string>
#include "transit-files/gtfs-realtime.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

using namespace std;
using namespace transit_realtime;

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Open the GTFS-realtime feed
    fstream input("Convert.pb", ios::in | ios::binary);
    if (!input) {
        cerr << "Error: could not open TripUpdates.pb" << endl;
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
    options.always_print_primitive_fields = true;
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