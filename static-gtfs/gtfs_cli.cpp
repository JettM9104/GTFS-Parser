#include "gtfs.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <libgen.h>
#include <climits>
#include <cstdlib>

using std::cout, std::cerr, std::string, std::endl;
using std::vector;

struct Options {
    bool json = false;
    int precision = 6;
};

// --- config helpers ---

static string configFilePath(const char* argv0) {
    char resolved[PATH_MAX];
    char input[PATH_MAX];
    // copy so dirname can modify it
    strncpy(input, argv0, PATH_MAX - 1);
    input[PATH_MAX - 1] = '\0';
    if (realpath(argv0, resolved)) {
        char copy[PATH_MAX];
        strncpy(copy, resolved, PATH_MAX - 1);
        copy[PATH_MAX - 1] = '\0';
        return string(dirname(copy)) + "/config.json";
    }
    // fallback: directory from argv[0] as-is
    return string(dirname(input)) + "/config.json";
}

static string readConfigDataPath(const string& cfgFile) {
    std::ifstream f(cfgFile);
    if (!f.is_open()) return "";
    string content, line;
    while (getline(f, line)) content += line;
    auto pos = content.find("\"data_path\"");
    if (pos == string::npos) return "";
    pos = content.find('"', pos + 11);  // opening quote of value
    if (pos == string::npos) return "";
    ++pos;
    auto end = content.find('"', pos);
    if (end == string::npos) return "";
    return content.substr(pos, end - pos);
}

static void writeConfigDataPath(const string& cfgFile, const string& dataPath) {
    std::ofstream f(cfgFile);
    if (!f.is_open()) throw std::runtime_error("cannot write config: " + cfgFile);
    f << "{\n    \"data_path\": \"" << dataPath << "\"\n}\n";
}

static void applyRoot(const string& newRoot) {
    string r = newRoot;
    if (!r.empty() && r.back() != '/') r += '/';
    gtfs::root          = r;
    gtfs::stopPath      = r + "stops.txt";
    gtfs::routePath     = r + "routes.txt";
    gtfs::tripsPath     = r + "trips.txt";
    gtfs::stopTimesPath = r + "stop_times.txt";
    gtfs::tripPath      = r + "trips.txt";
    gtfs::calendarPath  = r + "calendar.txt";
    gtfs::calendarDatesPath = r + "calendar_dates.txt";
    gtfs::agencyPath    = r + "agency.txt";
    gtfs::shapePath     = r + "shapes.txt";
    gtfs::feedInfoFile  = r + "feed_info.txt";
}

// --- string helpers ---

static string allowableStr(gtfs::trip::allowable a) {
    switch (a) {
        case gtfs::trip::allowable::no_info:     return "no_info";
        case gtfs::trip::allowable::allowed:     return "allowed";
        case gtfs::trip::allowable::not_allowed: return "not_allowed";
        default: return "unknown";
    }
}

static string routeTypeStr(gtfs::route::type t) {
    switch (t) {
        case gtfs::route::type::light_rail:  return "light_rail";
        case gtfs::route::type::underground: return "underground";
        case gtfs::route::type::rail:        return "rail";
        case gtfs::route::type::bus:         return "bus";
        case gtfs::route::type::ferry:       return "ferry";
        case gtfs::route::type::cable_tram:  return "cable_tram";
        case gtfs::route::type::aerial_lift: return "aerial_lift";
        case gtfs::route::type::funicular:   return "funicular";
        case gtfs::route::type::trolleybus:  return "trolleybus";
        case gtfs::route::type::monorail:    return "monorail";
        default: return "unknown";
    }
}

static string feedStatusStr(gtfs::feedStatus fs) {
    switch (fs) {
        case gtfs::feedStatus::in_use:    return "in_use";
        case gtfs::feedStatus::expired:   return "expired";
        case gtfs::feedStatus::upcoming:  return "upcoming";
        default: return "no_result";
    }
}

static string jstr(const string& s) {
    string out;
    out.reserve(s.size() + 2);
    for (char c : s) {
        if      (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else                out += c;
    }
    return out;
}

static string fmtDate(int y, int m, int d) {
    return std::to_string(y) + "-" +
           (m < 10 ? "0" : "") + std::to_string(m) + "-" +
           (d < 10 ? "0" : "") + std::to_string(d);
}

static string fmtDate(const gtfs::calendar_day& cd) {
    return fmtDate(cd.year, cd.month, cd.day);
}

// --- subcommands ---

int cmdTrip(const Options& opts, int argc, char* argv[]) {
    if (argc < 1) {
        cerr << "Usage:\n"
             << "  trip <trip_id>\n"
             << "  trip --stops <trip_id>\n"
             << "  trip --route <route_id> <year> <month> <day>\n"
             << "  trip --block <block_id>\n";
        return 1;
    }

    string sub = argv[0];

    if (sub == "--stops") {
        if (argc < 2) { cerr << "Usage: trip --stops <trip_id>\n"; return 1; }
        string trip_id = argv[1];
        auto segs = gtfs::getAllStops(trip_id);
        cout << std::fixed << std::setprecision(opts.precision);
        if (opts.json) {
            cout << "{\n\t\"trip_id\": \"" << jstr(trip_id) << "\",\n"
                 << "\t\"count\": " << segs.size() << ",\n"
                 << "\t\"stops\": [\n";
            for (int i = 0; i < (int)segs.size(); i++) {
                auto& seg = segs[i];
                auto st = gtfs::getStopInfo(seg.stop.stop_id);
                cout << "\t\t{ \"sequence\": " << seg.stop.stop_sequence
                     << ", \"stop_id\": \"" << jstr(seg.stop.stop_id) << "\""
                     << ", \"stop_name\": \"" << jstr(st.stop_name) << "\""
                     << ", \"arrival\": \"" << seg.stop.arrival_time.leadingRoundedTime() << "\""
                     << ", \"departure\": \"" << seg.stop.departure_time.leadingRoundedTime() << "\""
                     << ", \"lat\": " << st.stop_lat
                     << ", \"lon\": " << st.stop_lon
                     << (i == (int)segs.size() - 1 ? " }\n" : " },\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Trip " << trip_id << " — " << segs.size() << " stops:\n\n";
            for (auto& seg : segs) {
                auto st = gtfs::getStopInfo(seg.stop.stop_id);
                cout << "  [" << std::setw(3) << seg.stop.stop_sequence << "]  "
                     << seg.stop.arrival_time.leadingRoundedTime()
                     << "  " << st.stop_name
                     << " (" << seg.stop.stop_id << ")\n";
            }
        }
        return 0;
    }

    if (sub == "--route") {
        if (argc < 5) { cerr << "Usage: trip --route <route_id> <year> <month> <day>\n"; return 1; }
        string route_id = argv[1];
        int year = std::stoi(argv[2]), month = std::stoi(argv[3]), day = std::stoi(argv[4]);
        auto trips = gtfs::getAllTrips(route_id);
        vector<gtfs::trip> valid;
        for (auto& t : trips) {
            if (gtfs::isTripValid(t.trip_id, year, month, day))
                valid.push_back(gtfs::getTripInfo(t.trip_id));
        }
        if (opts.json) {
            cout << "{\n\t\"route_id\": \"" << jstr(route_id) << "\",\n"
                 << "\t\"date\": \"" << fmtDate(year, month, day) << "\",\n"
                 << "\t\"count\": " << valid.size() << ",\n"
                 << "\t\"trips\": [\n";
            for (int i = 0; i < (int)valid.size(); i++) {
                auto& t = valid[i];
                cout << "\t\t{ \"trip_id\": \"" << jstr(t.trip_id) << "\""
                     << ", \"headsign\": \"" << jstr(t.trip_headsign) << "\""
                     << ", \"direction\": " << (t.direction_id ? 1 : 0)
                     << ", \"service_id\": \"" << jstr(t.service_id) << "\""
                     << (i == (int)valid.size() - 1 ? " }\n" : " },\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Route " << route_id << " on " << fmtDate(year, month, day)
                 << " — " << valid.size() << " trips:\n\n";
            for (auto& t : valid) {
                cout << "  " << t.trip_id
                     << "  dir=" << t.direction_id
                     << "  " << t.trip_headsign << "\n";
            }
        }
        return 0;
    }

    if (sub == "--block") {
        if (argc < 2) { cerr << "Usage: trip --block <block_id>\n"; return 1; }
        string block_id = argv[1];
        auto trips = gtfs::getAllBlockId(block_id);
        if (opts.json) {
            cout << "{\n\t\"block_id\": \"" << jstr(block_id) << "\",\n"
                 << "\t\"count\": " << trips.size() << ",\n"
                 << "\t\"trips\": [\n";
            for (int i = 0; i < (int)trips.size(); i++) {
                cout << "\t\t\"" << jstr(trips[i].trip_id) << "\""
                     << (i == (int)trips.size() - 1 ? "\n" : ",\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Block " << block_id << " — " << trips.size() << " trips:\n\n";
            for (auto& t : trips) cout << "  " << t.trip_id << "\n";
        }
        return 0;
    }

    // basic trip info
    {
        string trip_id = sub;
        auto info  = gtfs::getTripInfo(trip_id);
        auto route = gtfs::getRouteInfo(info.route_id);
        if (opts.json) {
            cout << "{\n"
                 << "\t\"trip_id\": \""       << jstr(info.trip_id)       << "\",\n"
                 << "\t\"route_id\": \""      << jstr(info.route_id)      << "\",\n"
                 << "\t\"service_id\": \""    << jstr(info.service_id)    << "\",\n"
                 << "\t\"headsign\": \""      << jstr(info.trip_headsign) << "\",\n"
                 << "\t\"direction\": "       << (info.direction_id ? 1 : 0)             << ",\n"
                 << "\t\"block_id\": \""      << jstr(info.block_id)      << "\",\n"
                 << "\t\"shape_id\": \""      << jstr(info.shape_id)      << "\",\n"
                 << "\t\"wheelchair\": \""    << allowableStr(info.wheelchair_accessible) << "\",\n"
                 << "\t\"bikes\": \""         << allowableStr(info.bikes_allowed)         << "\",\n"
                 << "\t\"route_short_name\": \"" << jstr(route.route_short_name) << "\",\n"
                 << "\t\"route_long_name\": \"" << jstr(route.route_long_name) << "\"\n"
                 << "}\n";
        } else {
            cout << "Trip:           " << info.trip_id << "\n"
                 << "Route:          " << info.route_id
                 << " (" << route.route_short_name << " — " << route.route_long_name << ")\n"
                 << "Service ID:     " << info.service_id << "\n"
                 << "Headsign:       " << info.trip_headsign << "\n"
                 << "Direction:      " << (info.direction_id ? "1 (inbound)" : "0 (outbound)") << "\n"
                 << "Block ID:       " << info.block_id << "\n"
                 << "Shape ID:       " << info.shape_id << "\n"
                 << "Wheelchair:     " << allowableStr(info.wheelchair_accessible) << "\n"
                 << "Bikes:          " << allowableStr(info.bikes_allowed) << "\n";
        }
    }
    return 0;
}

int cmdStop(const Options& opts, int argc, char* argv[]) {
    if (argc < 1) {
        cerr << "Usage:\n"
             << "  stop <stop_id>\n"
             << "  stop --departures <stop_id> [year month day]\n"
             << "  stop --remaining <stop_id>\n"
             << "  stop --nearby <lat> <lon> [-t N] [-d km]\n"
             << "  stop --search <name...>\n";
        return 1;
    }

    string sub = argv[0];

    if (sub == "--departures") {
        if (argc < 2) { cerr << "Usage: stop --departures <stop_id> [year month day]\n"; return 1; }
        string stop_id = argv[1];
        int year, month, day;
        if (argc >= 5) {
            year = std::stoi(argv[2]); month = std::stoi(argv[3]); day = std::stoi(argv[4]);
        } else {
            auto today = gtfs::getToday();
            year = today.year; month = today.month; day = today.day;
        }
        auto st   = gtfs::getStopInfo(stop_id);
        auto deps = gtfs::getDayTimesAtStop(stop_id, year, month, day);
        if (opts.json) {
            cout << "{\n"
                 << "\t\"stop_id\": \""   << jstr(stop_id)    << "\",\n"
                 << "\t\"stop_name\": \"" << jstr(st.stop_name) << "\",\n"
                 << "\t\"date\": \""      << fmtDate(year, month, day) << "\",\n"
                 << "\t\"count\": " << deps.size() << ",\n"
                 << "\t\"departures\": [\n";
            for (int i = 0; i < (int)deps.size(); i++) {
                auto& seg   = deps[i];
                auto  route = gtfs::getRouteInfo(seg.route_id);
                cout << "\t\t{ \"route\": \""    << jstr(route.route_short_name) << "\""
                     << ", \"arrival\": \""       << seg.stop.arrival_time.leadingRoundedTime() << "\""
                     << ", \"departure\": \""     << seg.stop.departure_time.leadingRoundedTime() << "\""
                     << ", \"trip_id\": \""       << jstr(seg.stop.trip_id) << "\""
                     << ", \"route_color\": \"#" << jstr(route.route_color) << "\""
                     << (i == (int)deps.size() - 1 ? " }\n" : " },\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Stop " << stop_id << " — " << st.stop_name << "\n"
                 << "Departures on " << fmtDate(year, month, day)
                 << " (" << deps.size() << " total):\n\n";
            for (auto& seg : deps) {
                auto route = gtfs::getRouteInfo(seg.route_id);
                cout << "  " << seg.stop.arrival_time.leadingRoundedTime()
                     << "  " << std::left << std::setw(6) << route.route_short_name
                     << "  " << seg.stop.trip_id << "\n";
            }
        }
        return 0;
    }

    if (sub == "--remaining") {
        if (argc < 2) { cerr << "Usage: stop --remaining <stop_id>\n"; return 1; }
        string stop_id = argv[1];
        auto now   = gtfs::getCurrentTime();
        auto today = gtfs::getToday();
        auto st    = gtfs::getStopInfo(stop_id);
        auto deps  = gtfs::getRemainingDayStops(stop_id, now, today);
        if (opts.json) {
            cout << "{\n"
                 << "\t\"stop_id\": \""    << jstr(stop_id)                  << "\",\n"
                 << "\t\"stop_name\": \""  << jstr(st.stop_name)             << "\",\n"
                 << "\t\"from_time\": \""  << now.leadingRoundedTime()        << "\",\n"
                 << "\t\"count\": " << deps.size() << ",\n"
                 << "\t\"departures\": [\n";
            for (int i = 0; i < (int)deps.size(); i++) {
                auto& seg   = deps[i];
                auto  route = gtfs::getRouteInfo(seg.route_id);
                cout << "\t\t{ \"route\": \""   << jstr(route.route_short_name) << "\""
                     << ", \"arrival\": \""       << seg.stop.arrival_time.leadingRoundedTime() << "\""
                     << ", \"trip_id\": \""       << jstr(seg.stop.trip_id) << "\""
                     << ", \"route_color\": \"#" << jstr(route.route_color) << "\""
                     << (i == (int)deps.size() - 1 ? " }\n" : " },\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Stop " << stop_id << " — " << st.stop_name << "\n"
                 << "Remaining departures after " << now.leadingRoundedTime()
                 << " (" << deps.size() << " total):\n\n";
            for (auto& seg : deps) {
                auto route = gtfs::getRouteInfo(seg.route_id);
                cout << "  " << seg.stop.arrival_time.leadingRoundedTime()
                     << "  " << std::left << std::setw(6) << route.route_short_name
                     << "  " << seg.stop.trip_id << "\n";
            }
        }
        return 0;
    }

    if (sub == "--nearby") {
        if (argc < 3) { cerr << "Usage: stop --nearby <lat> <lon> [-t N] [-d km]\n"; return 1; }
        double lat = std::stod(argv[1]);
        double lon = std::stod(argv[2]);
        int    top     = 10;
        double maxDist = -1;
        for (int i = 3; i < argc; i++) {
            string arg = argv[i];
            if ((arg == "-t" || arg == "--total") && i + 1 < argc)
                top = std::stoi(argv[++i]);
            else if ((arg == "-d" || arg == "--distance") && i + 1 < argc)
                maxDist = std::stod(argv[++i]);
        }
        auto stops = gtfs::getNearestStops(lat, lon, top, maxDist);
        cout << std::fixed << std::setprecision(opts.precision);
        if (opts.json) {
            cout << "{\n\t\"lat\": " << lat << ", \"lon\": " << lon << ",\n"
                 << "\t\"count\": " << stops.size() << ",\n"
                 << "\t\"stops\": [\n";
            for (int i = 0; i < (int)stops.size(); i++) {
                auto& s    = stops[i];
                double dist = gtfs::getDistanceKM(lat, lon, s.stop_lat, s.stop_lon);
                cout << "\t\t{ \"stop_id\": \""   << jstr(s.stop_id) << "\""
                     << ", \"stop_name\": \""      << jstr(s.stop_name) << "\""
                     << ", \"lat\": "              << s.stop_lat
                     << ", \"lon\": "              << s.stop_lon
                     << ", \"distanceKM\": "       << dist
                     << (i == (int)stops.size() - 1 ? " }\n" : " },\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Nearest stops to (" << lat << ", " << lon << "):\n\n";
            for (auto& s : stops) {
                double dist = gtfs::getDistanceKM(lat, lon, s.stop_lat, s.stop_lon);
                cout << "  " << std::left << std::setw(10) << s.stop_id
                     << std::right << std::setw(8) << std::setprecision(3) << dist << " km"
                     << "  " << s.stop_name << "\n";
            }
        }
        return 0;
    }

    if (sub == "--search") {
        if (argc < 2) { cerr << "Usage: stop --search <name...>\n"; return 1; }
        // join remaining args as the search query
        string name = argv[1];
        for (int i = 2; i < argc; i++) { name += " "; name += argv[i]; }
        auto results = gtfs::searchStop(name);
        int top = std::min(10, (int)results.size());
        if (opts.json) {
            cout << "{\n\t\"query\": \"" << jstr(name) << "\",\n"
                 << "\t\"results\": [\n";
            for (int i = 0; i < top; i++) {
                auto& r = results[i];
                cout << "\t\t{ \"stop_id\": " << r.text.num
                     << ", \"stop_name\": \"" << jstr(r.text.str) << "\""
                     << ", \"score\": " << r.score
                     << (i == top - 1 ? " }\n" : " },\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Search results for \"" << name << "\":\n\n";
            for (int i = 0; i < top; i++) {
                auto& r = results[i];
                cout << "  " << std::left << std::setw(10) << r.text.num
                     << std::right << std::setw(3) << r.score << "%  "
                     << r.text.str << "\n";
            }
        }
        return 0;
    }

    // basic stop info
    {
        string stop_id = sub;
        auto st = gtfs::getStopInfo(stop_id);
        cout << std::fixed << std::setprecision(opts.precision);
        if (opts.json) {
            cout << "{\n"
                 << "\t\"stop_id\": \""       << jstr(st.stop_id)       << "\",\n"
                 << "\t\"stop_code\": \""     << jstr(st.stop_code)     << "\",\n"
                 << "\t\"stop_name\": \""     << jstr(st.stop_name)     << "\",\n"
                 << "\t\"lat\": "             << st.stop_lat             << ",\n"
                 << "\t\"lon\": "             << st.stop_lon             << ",\n"
                 << "\t\"zone_id\": \""       << jstr(st.zone_id)       << "\",\n"
                 << "\t\"parent_station\": \"" << jstr(st.parent_station) << "\"\n"
                 << "}\n";
        } else {
            cout << "Stop ID:        " << st.stop_id << "\n"
                 << "Stop Code:      " << st.stop_code << "\n"
                 << "Name:           " << st.stop_name << "\n"
                 << "Coordinates:    " << st.stop_lat << ", " << st.stop_lon << "\n"
                 << "Zone:           " << st.zone_id << "\n"
                 << "Parent Station: " << (st.parent_station.empty() ? "(none)" : st.parent_station) << "\n";
        }
    }
    return 0;
}

int cmdRoute(const Options& opts, int argc, char* argv[]) {
    if (argc < 1) { cerr << "Usage: route <route_id>\n"; return 1; }
    auto r = gtfs::getRouteInfo(argv[0]);
    if (opts.json) {
        cout << "{\n"
             << "\t\"route_id\": \""    << jstr(r.route_id)         << "\",\n"
             << "\t\"agency_id\": \""   << jstr(r.agency_id)        << "\",\n"
             << "\t\"short_name\": \""  << jstr(r.route_short_name) << "\",\n"
             << "\t\"long_name\": \""   << jstr(r.route_long_name)  << "\",\n"
             << "\t\"type\": \""        << routeTypeStr(r.route_type) << "\",\n"
             << "\t\"color\": \"#"     << jstr(r.route_color)       << "\",\n"
             << "\t\"text_color\": \"#" << jstr(r.route_text_color)  << "\"\n"
             << "}\n";
    } else {
        cout << "Route ID:    " << r.route_id << "\n"
             << "Agency:      " << r.agency_id << "\n"
             << "Short Name:  " << r.route_short_name << "\n"
             << "Long Name:   " << r.route_long_name << "\n"
             << "Type:        " << routeTypeStr(r.route_type) << "\n"
             << "Color:       #" << r.route_color << " / #" << r.route_text_color << "\n";
    }
    return 0;
}

int cmdAgency(const Options& opts, int /*argc*/, char* /*argv*/[]) {
    auto agencies = gtfs::getAgencyInfo();
    if (opts.json) {
        cout << "[\n";
        for (int i = 0; i < (int)agencies.size(); i++) {
            auto& a = agencies[i];
            cout << "\t{\n"
                 << "\t\t\"agency_id\": \""  << jstr(a.agency_id)       << "\",\n"
                 << "\t\t\"name\": \""       << jstr(a.agency_name)     << "\",\n"
                 << "\t\t\"url\": \""        << jstr(a.agency_url)      << "\",\n"
                 << "\t\t\"timezone\": \""   << jstr(a.agency_timezone) << "\",\n"
                 << "\t\t\"lang\": \""       << jstr(a.agency_lang)     << "\",\n"
                 << "\t\t\"phone\": \""      << jstr(a.agency_phone)    << "\"\n"
                 << "\t}" << (i == (int)agencies.size() - 1 ? "\n" : ",\n");
        }
        cout << "]\n";
    } else {
        for (auto& a : agencies) {
            cout << "Agency ID:  " << a.agency_id   << "\n"
                 << "Name:       " << a.agency_name << "\n"
                 << "URL:        " << a.agency_url  << "\n"
                 << "Timezone:   " << a.agency_timezone << "\n"
                 << "Phone:      " << a.agency_phone << "\n\n";
        }
    }
    return 0;
}

int cmdShape(const Options& opts, int argc, char* argv[]) {
    if (argc < 1) { cerr << "Usage: shape <shape_id>\n"; return 1; }
    string shape_id = argv[0];
    auto pts = gtfs::getShapeInfo(shape_id);
    cout << std::fixed << std::setprecision(opts.precision);
    if (opts.json) {
        cout << "{\n\t\"shape_id\": \"" << jstr(shape_id) << "\",\n"
             << "\t\"count\": " << pts.size() << ",\n"
             << "\t\"points\": [\n";
        for (int i = 0; i < (int)pts.size(); i++) {
            auto& p = pts[i];
            cout << "\t\t{ \"seq\": " << p.shape_pt_sequence
                 << ", \"lat\": " << p.shape_pt_lat
                 << ", \"lon\": " << p.shape_pt_lon
                 << (i == (int)pts.size() - 1 ? " }\n" : " },\n");
        }
        cout << "\t]\n}\n";
    } else {
        cout << "Shape " << shape_id << " — " << pts.size() << " points:\n\n";
        for (auto& p : pts) {
            cout << "  [" << std::setw(5) << p.shape_pt_sequence << "]  "
                 << p.shape_pt_lat << ", " << p.shape_pt_lon << "\n";
        }
    }
    return 0;
}

int cmdService(const Options& opts, int argc, char* argv[]) {
    if (argc < 1) {
        cerr << "Usage:\n"
             << "  service <service_id>\n"
             << "  service --valid <trip_id> [year month day]\n"
             << "  service --verify [year month day]\n";
        return 1;
    }

    string sub = argv[0];

    if (sub == "--valid") {
        if (argc < 2) { cerr << "Usage: service --valid <trip_id> [year month day]\n"; return 1; }
        string trip_id = argv[1];
        int year, month, day;
        if (argc >= 5) {
            year = std::stoi(argv[2]); month = std::stoi(argv[3]); day = std::stoi(argv[4]);
        } else {
            auto today = gtfs::getToday();
            year = today.year; month = today.month; day = today.day;
        }
        bool valid = gtfs::isTripValid(trip_id, year, month, day);
        if (opts.json) {
            cout << "{ \"trip_id\": \"" << jstr(trip_id) << "\""
                 << ", \"date\": \"" << fmtDate(year, month, day) << "\""
                 << ", \"valid\": " << (valid ? "true" : "false") << " }\n";
        } else {
            cout << "Trip " << trip_id << " on " << fmtDate(year, month, day)
                 << ": " << (valid ? "VALID" : "NOT VALID") << "\n";
        }
        return 0;
    }

    if (sub == "--verify") {
        int year, month, day;
        if (argc >= 4) {
            year = std::stoi(argv[1]); month = std::stoi(argv[2]); day = std::stoi(argv[3]);
        } else {
            auto today = gtfs::getToday();
            year = today.year; month = today.month; day = today.day;
        }
        auto status = gtfs::verifyGTFS(year, month, day);
        if (opts.json) {
            cout << "{ \"date\": \"" << fmtDate(year, month, day) << "\""
                 << ", \"status\": \"" << feedStatusStr(status) << "\" }\n";
        } else {
            cout << "GTFS feed on " << fmtDate(year, month, day)
                 << ": " << feedStatusStr(status) << "\n";
        }
        return 0;
    }

    // service <service_id>
    {
        string service_id = sub;
        auto svc = gtfs::getServiceInfo(service_id);
        auto& cal = svc.schedule;
        if (opts.json) {
            cout << "{\n"
                 << "\t\"service_id\": \"" << jstr(service_id) << "\",\n"
                 << "\t\"start_date\": \"" << fmtDate(cal.start_date) << "\",\n"
                 << "\t\"end_date\": \""   << fmtDate(cal.end_date)   << "\",\n"
                 << "\t\"schedule\": {\n"
                 << "\t\t\"mon\": " << (cal.monday    ? "true" : "false") << ",\n"
                 << "\t\t\"tue\": " << (cal.tuesday   ? "true" : "false") << ",\n"
                 << "\t\t\"wed\": " << (cal.wednesday ? "true" : "false") << ",\n"
                 << "\t\t\"thu\": " << (cal.thursday  ? "true" : "false") << ",\n"
                 << "\t\t\"fri\": " << (cal.friday    ? "true" : "false") << ",\n"
                 << "\t\t\"sat\": " << (cal.saturday  ? "true" : "false") << ",\n"
                 << "\t\t\"sun\": " << (cal.sunday    ? "true" : "false") << "\n"
                 << "\t},\n"
                 << "\t\"exceptions\": [\n";
            for (int i = 0; i < (int)svc.exceptions.size(); i++) {
                auto& ex = svc.exceptions[i];
                cout << "\t\t{ \"date\": \"" << fmtDate(ex.date) << "\""
                     << ", \"type\": \"" << (ex.exception_type == gtfs::calendar_date::added ? "added" : "removed") << "\""
                     << (i == (int)svc.exceptions.size() - 1 ? " }\n" : " },\n");
            }
            cout << "\t]\n}\n";
        } else {
            cout << "Service ID:  " << service_id << "\n"
                 << "Active:      " << fmtDate(cal.start_date) << " to " << fmtDate(cal.end_date) << "\n"
                 << "Schedule:    "
                 << (cal.monday    ? "Mon " : "--- ")
                 << (cal.tuesday   ? "Tue " : "--- ")
                 << (cal.wednesday ? "Wed " : "--- ")
                 << (cal.thursday  ? "Thu " : "--- ")
                 << (cal.friday    ? "Fri " : "--- ")
                 << (cal.saturday  ? "Sat " : "--- ")
                 << (cal.sunday    ? "Sun"  : "---") << "\n";
            if (!svc.exceptions.empty()) {
                cout << "Exceptions (" << svc.exceptions.size() << "):\n";
                for (auto& ex : svc.exceptions) {
                    cout << "  " << fmtDate(ex.date) << "  "
                         << (ex.exception_type == gtfs::calendar_date::added ? "added" : "removed") << "\n";
                }
            }
        }
    }
    return 0;
}

int cmdDistance(const Options& opts, int argc, char* argv[]) {
    if (argc < 4) { cerr << "Usage: distance <lat1> <lon1> <lat2> <lon2>\n"; return 1; }
    double lat1 = std::stod(argv[0]), lon1 = std::stod(argv[1]);
    double lat2 = std::stod(argv[2]), lon2 = std::stod(argv[3]);
    double dist = gtfs::getDistanceKM(lat1, lon1, lat2, lon2);
    cout << std::fixed << std::setprecision(opts.precision);
    if (opts.json) {
        cout << "{ \"lat1\": " << lat1 << ", \"lon1\": " << lon1
             << ", \"lat2\": " << lat2 << ", \"lon2\": " << lon2
             << ", \"distanceKM\": " << dist << " }\n";
    } else {
        cout << "Distance: " << dist << " km\n";
    }
    return 0;
}

static const string kDefaultRoot =
    "/Users/jettmu/Documents/VSCode/GTFS Parser/static-gtfs/data/yrt_archive/";

int cmdConfig(const Options& /*opts*/, int argc, char* argv[], const string& cfgFile) {
    if (argc < 1) {
        cerr << "Usage:\n"
             << "  config get              Show current data path\n"
             << "  config set <path>       Save data path to config.json\n"
             << "  config reset            Remove config.json (restore default)\n";
        return 1;
    }

    string sub = argv[0];

    if (sub == "get") {
        string saved = readConfigDataPath(cfgFile);
        cout << "Config file:  " << cfgFile << "\n"
             << "Data path:    " << (saved.empty() ? gtfs::root + " (default)" : saved) << "\n";
        return 0;
    }

    if (sub == "set") {
        if (argc < 2) { cerr << "Usage: config set <path>\n"; return 1; }
        string newPath = argv[1];
        // ensure trailing slash
        if (!newPath.empty() && newPath.back() != '/') newPath += '/';
        writeConfigDataPath(cfgFile, newPath);
        cout << "Saved data path: " << newPath << "\n"
             << "Config file:     " << cfgFile << "\n";
        return 0;
    }

    if (sub == "reset") {
        if (std::remove(cfgFile.c_str()) == 0)
            cout << "Removed " << cfgFile << " (using default path)\n";
        else
            cout << "No config file found; already using default path.\n";
        return 0;
    }

    cerr << "Unknown config subcommand: " << sub << "\n";
    return 1;
}

void printHelp(const string& prog) {
    cerr << "Usage: " << prog << " [--json|-j] [--precision|-p N] [-c <path>] <command> [args...]\n"
         << "\n"
         << "Trip commands:\n"
         << "  trip <trip_id>                            Basic trip info\n"
         << "  trip --stops <trip_id>                    All stops along a trip\n"
         << "  trip --route <route_id> <y> <m> <d>       Valid trips for a route on a date\n"
         << "  trip --block <block_id>                   All trips sharing a block\n"
         << "\n"
         << "Stop commands:\n"
         << "  stop <stop_id>                            Basic stop info\n"
         << "  stop --departures <stop_id> [y m d]       All departures at a stop\n"
         << "  stop --remaining <stop_id>                Remaining departures from now\n"
         << "  stop --nearby <lat> <lon> [-t N] [-d km]  Nearest stops\n"
         << "  stop --search <name...>                   Search stops by name\n"
         << "\n"
         << "Other commands:\n"
         << "  route <route_id>                          Route details\n"
         << "  agency                                    Agency info\n"
         << "  shape <shape_id>                          Shape geographic points\n"
         << "  service <service_id>                      Service calendar with exceptions\n"
         << "  service --valid <trip_id> [y m d]         Check if a trip runs on a date\n"
         << "  service --verify [y m d]                  Verify GTFS feed status\n"
         << "  distance <lat1> <lon1> <lat2> <lon2>      Haversine distance in km\n"
         << "\n"
         << "Config commands:\n"
         << "  config get                                Show active data path\n"
         << "  config set <path>                         Save data path to config.json\n"
         << "  config reset                              Remove config.json (restore default)\n"
         << "\n"
         << "Options:\n"
         << "  --json, -j           Output JSON instead of plain text\n"
         << "  --precision, -p N    Decimal places for coordinates (default: 6)\n"
         << "  -c, --config <path>  Override data path for this run (does not save)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { printHelp(argv[0]); return 0; }

    string cfgFile = configFilePath(argv[0]);

    // auto-load saved config
    string savedPath = readConfigDataPath(cfgFile);
    if (!savedPath.empty()) applyRoot(savedPath);

    Options opts;
    int cmdIdx = 1;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--json" || arg == "-j") {
            opts.json = true;
            cmdIdx = i + 1;
        } else if ((arg == "--precision" || arg == "-p") && i + 1 < argc) {
            opts.precision = std::stoi(argv[++i]);
            cmdIdx = i + 1;
        } else if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
            applyRoot(argv[++i]);   // per-run override, does not save
            cmdIdx = i + 1;
        } else if (arg == "--help" || arg == "-h") {
            printHelp(argv[0]);
            return 0;
        } else {
            cmdIdx = i;
            break;
        }
    }

    if (cmdIdx >= argc) { printHelp(argv[0]); return 0; }

    string cmd     = argv[cmdIdx];
    int    subArgc = argc - cmdIdx - 1;
    char** subArgv = argv + cmdIdx + 1;

    try {
        if (cmd == "trip")     return cmdTrip(opts, subArgc, subArgv);
        if (cmd == "stop")     return cmdStop(opts, subArgc, subArgv);
        if (cmd == "route")    return cmdRoute(opts, subArgc, subArgv);
        if (cmd == "agency")   return cmdAgency(opts, subArgc, subArgv);
        if (cmd == "shape")    return cmdShape(opts, subArgc, subArgv);
        if (cmd == "service")  return cmdService(opts, subArgc, subArgv);
        if (cmd == "distance") return cmdDistance(opts, subArgc, subArgv);
        if (cmd == "config")   return cmdConfig(opts, subArgc, subArgv, cfgFile);
        cerr << "Unknown command: " << cmd << "\n";
        printHelp(argv[0]);
        return 1;
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
