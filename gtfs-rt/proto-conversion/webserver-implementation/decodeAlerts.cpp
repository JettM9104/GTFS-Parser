#include <iostream>
#include <fstream>
#include <string>
#include "../transit-files/gtfs-realtime.pb.h"
#include <google/protobuf/util/json_util.h>
#include <libgen.h>

/* build command
clang++ -std=c++17 decodeAlerts.cpp ../transit-files/gtfs-realtime.pb.cc -I/opt/homebrew/opt/protobuf@21/include -L/opt/homebrew/opt/protobuf@21/lib -lprotobuf -o decodeAlerts -O3
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
    options.always_print_primitive_fields = true;
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
        google::protobuf::util::MessageToJsonString(entity, &entityJson, options);
        cout << entityJson << "\n";
        foundAny = true;
    }

    if (!foundAny) {
        cerr << "No alerts found for route: " << routeId << endl;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}