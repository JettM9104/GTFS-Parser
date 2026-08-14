#include <iostream>
#include <fstream>
#include <string>
#include "../transit-files/gtfs-realtime.pb.h"
#include <google/protobuf/util/json_util.h>
#include <libgen.h>

/* build command
clang++ -std=c++17 -O3 decodeAlerts.cpp ../transit-files/gtfs-realtime.pb.cc $(pkg-config --cflags --libs protobuf) -o decodeAlerts
*/
using namespace std;
using namespace transit_realtime;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <route_id>" << endl;
        return 1;
    }

    string routeId = argv[1];
    string exeDir = dirname(argv[0]);
    string outputPath = exeDir + "/downloaded_alerts.pb";

    string cmd = "wget -q -O " + outputPath + " https://storage.googleapis.com/transit-terminal-alerts-bucket-production/yrt.pb";
    system(cmd.c_str());

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    fstream input(outputPath, ios::in | ios::binary);
    if (!input) {
        cerr << "Error: could not open " << outputPath << endl;
        return 1;
    }

    FeedMessage feed;
    if (!feed.ParseFromIstream(&input)) {
        cerr << "Error: failed to parse GTFS-realtime data" << endl;
        return 1;
    }

    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_fields_with_no_presence = true;
    options.preserve_proto_field_names = true;

    bool foundAny = false;
    for (const FeedEntity& entity : feed.entity()) {
        if (!entity.has_alert()) continue;

        const Alert& alert = entity.alert();
        bool matchesRoute = false;

        for (const EntitySelector& selector : alert.informed_entity()) {
            if (selector.route_id() == routeId) {
                matchesRoute = true;
                break;
            }
        }

        if (!matchesRoute) continue;

        string entityJson;
        auto status = google::protobuf::util::MessageToJsonString(entity, &entityJson, options);
        if (!status.ok()) {
            cerr << "Error: failed to convert alert to JSON: " << status.ToString() << endl;
            continue;
        }
        cout << entityJson << "\n";
        foundAny = true;
    }

    if (!foundAny) {
        cerr << "No alerts found for route: " << routeId << endl;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}