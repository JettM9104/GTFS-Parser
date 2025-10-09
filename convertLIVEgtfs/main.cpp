#include <iostream>
#include <fstream>
#include <string>
#include "gtfs-realtime.pb.h"
#include <google/protobuf/text_format.h>

using namespace std;
using namespace transit_realtime;

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Open the GTFS-realtime feed
    fstream input("TripUpdates.pb", ios::in | ios::binary);
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
    ofstream outfile("decoded_gtfs.txt");
    if (!outfile) {
        cerr << "Error: could not open output file" << endl;
        return 1;
    }

    // Convert to readable text format
    string output;
    google::protobuf::TextFormat::PrintToString(feed, &output);

    // Write to file
    outfile << output;
    outfile.close();

    cout << "✅ GTFS data successfully written to decoded_gtfs.txt" << endl;

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
