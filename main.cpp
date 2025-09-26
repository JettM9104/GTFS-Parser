#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using std::cout, std::string, std::ifstream, std::ofstream, std::stoi;

typedef enum {ident, code} stopType;
typedef enum {am, pm} tod;
typedef enum {mon, tue, wed, thu, fri, sat, sun} week;

string stopPath = "data/yrt_archive/stops.txt";
string routePath = "data/yrt_archive/routes.txt";
string tripsPath = "data/yrt_archive/trips.txt";
string stopTimesPath = "data/yrt_archive/stop_times.txt";

struct busLine {
    int route_id;
    string agency_id;
    string route_short_name;
    string route_long_name;
    string route_desc;
    string route_type;
    string route_url;
    string route_color;
    string route_text_color;

    void printInfo() const {
        cout << "Route ID: " << route_id << std::endl;
        cout << "Agency ID: " << agency_id << std::endl;
        cout << "Route Short Name: " << route_short_name << std::endl;
        cout << "Route Long Name: " << route_long_name << std::endl;
        cout << "Route Description: " << route_desc << std::endl;
        cout << "Route Type: " << route_type << std::endl;
        cout << "Route URL: " << route_url << std::endl;
        cout << "Route Color: " << route_color << std::endl;
        cout << "Route Text Color: " << route_text_color << std::endl;
    }
};

struct stop {
    unsigned short int stop_id;
    unsigned short int stop_code;
    string stop_name;
    string stop_desc;
    unsigned short int platform_code;
    double stop_lat;
    double stop_lon;
    short int zone_id;
    string stop_url;
    short int location_type;
    short int parent_station;
    short int stop_timezone;
    short int wheelchair_boarding;
    short int preferred;

    void printInfo() const {
        cout << "Stop ID: " << stop_id << std::endl;
        cout << "Stop Code: " << stop_code << std::endl;
        cout << "Stop Name: " << stop_name << std::endl;
        cout << "Stop Description: " << stop_desc << std::endl;
        cout << "Platform Code: " << platform_code << std::endl;
        cout << "Latitude: " << stop_lat << std::endl;
        cout << "Longitude: " << stop_lon << std::endl;
        cout << "Zone ID: " << zone_id << std::endl;
        cout << "Stop URL: " << stop_url << std::endl;
        cout << "Location Type: " << location_type << std::endl;
        cout << "Parent Station: " << parent_station << std::endl;
        cout << "Stop Timezone: " << stop_timezone << std::endl;
        cout << "Wheelchair Boarding: " << wheelchair_boarding << std::endl;
        cout << "Preferred: " << preferred << std::endl;
    }
};

struct time24 {
    int h;
    int m;
    float s;
    virtual void printRawValues() const {
        cout << "h: " << h << std::endl;
        cout << "m: " << m << std::endl;
        cout << "s: " << s << std::endl;
    }
};

struct timeap : public time24 {
    tod timeOfDay;
    void printRawValues() const override {
        cout << "h: " << h << std::endl;
        cout << "m: " << m << std::endl;
        cout << "s: " << s << std::endl;
        cout << ((timeOfDay == am) ? "am" : "pm") << std::endl;
    }
};

struct tripSegment { // params from stop_times
    long trip_id;
    time24 arrival_time;
    time24 departure_time;
    int stop_id;
    int stop_sequence;
    int stop_headsign;
    int pickup_type;
    int drop_off_type;
    int shape_dist_traveled;
    int timepoint;
};

std::vector<string> parseDataCSV(const string& input);
std::map<string, int> createMapFromVector(std::vector<string> param);
time24 parseFormattedTime(string input);

std::vector<tripSegment> getDayTimesAtStop(week day, const unsigned short int& id) ;
busLine getRouteInfo(const unsigned short int& id);
busLine getRouteInfo(const string& id);
stop getStopInfo(const unsigned short int& id, const stopType& type);


int main(int argc, char* argv[]) {
    getDayTimesAtStop(mon, 6769);
}

time24 parseFormattedTime(string input) {
    time24 output;
    string h, m, s;
    short int segment = 1;
    for (int i = 0; i < input.length(); i++) {
        string character = input.substr(i, 1);
        if (character == ":") {
            segment++;
            continue;
        }
        switch (segment) {
            case 1:
                h += character;
                break;
            case 2:
                m += character;
                break;
            case 3:
                s += character;
                break;
        }

    }

    output.h = stoi(h);
    output.m = stoi(m);
    output.s = stoi(s);

    return output;
}

std::vector<string> parseDataCSV(const string& input) {
    std::vector<string> output;
    string additions;
    unsigned int length = input.length();

    for (int i = 0; i < length; i++) {
        if (input.substr(i, 1) == ",") {
            output.push_back(additions);
            additions = "";
            continue;
        }
        additions += input.substr(i, 1);

        if (i == length - 1) {
            output.push_back(additions);
            break;
        }
        
    }
    return output;
}

std::map<string, int> createMapFromVector(std::vector<string> param) {
    int length = param.size();
    std::map<string, int> output;
    for (int i = 0; i < length; i++) {
        if (param[i] == "" || param[i] == " ") {
            continue;
        } else { output[param[i]] = i; }
    }    
    return output;
}


busLine getRouteInfo(const unsigned short int& id) {
    ifstream routeFile(routePath);
    busLine bus;
    bus.route_id = id;
    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool bypass = false;

    while (getline(routeFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (parsedCurrentLine[0] == std::to_string(id)) { bypass = true; break; }
    }
    bus.route_id = stoi(parsedCurrentLine[0]);
    bus.agency_id = parsedCurrentLine[1];
    bus.route_short_name = parsedCurrentLine[2];
    bus.route_long_name = parsedCurrentLine[3];
    bus.route_desc = parsedCurrentLine[4];
    bus.route_type = parsedCurrentLine[5];
    bus.route_url = parsedCurrentLine[6];
    bus.route_color = parsedCurrentLine[7];
    bus.route_text_color = parsedCurrentLine[8];
    routeFile.close();
    return bus;
}

busLine getRouteInfo(const string& id) {
    ifstream routeFile(routePath);
    busLine bus;
    bus.route_short_name = id;
    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool bypass = false;

    while (getline(routeFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (parsedCurrentLine[2] == id) { bypass = true; break; }
    }

    bus.route_id = std::stoi(parsedCurrentLine[0]);
    bus.agency_id = parsedCurrentLine[1];
    bus.route_short_name = parsedCurrentLine[2];
    bus.route_long_name = parsedCurrentLine[3];
    bus.route_desc = parsedCurrentLine[4];
    bus.route_type = parsedCurrentLine[5];
    bus.route_url = parsedCurrentLine[6];
    bus.route_color = parsedCurrentLine[7];
    bus.route_text_color = parsedCurrentLine[8];
    routeFile.close();

    return bus;
}

stop getStopInfo(const unsigned short int& id, const stopType& type) {
    ifstream stopFile(stopPath);
    stop busStop;
    switch (type) {
        case ident:
            busStop.stop_id = id;
            break;
        case code:
            busStop.stop_code = id;
            break;
        default:
            busStop.stop_id = 0;
            busStop.stop_code = 0;
    }
    std::vector<string> parsedCurrentLine;
    string currentLine;
    bool bypass = false;
    if (type == ident) {
        unsigned int lineNumber = 0;
        std::vector<string> dataVector;
        while (getline(stopFile, currentLine)) {
            lineNumber++;
            if (lineNumber == 1) [[unlikely]] {
                dataVector = parseDataCSV(currentLine);
                for (int i = 0; i < dataVector.size(); i++) {
                    cout << dataVector[i] << std::endl;
                }
            }

            parsedCurrentLine = parseDataCSV(currentLine);
            if (parsedCurrentLine[0] == std::to_string(busStop.stop_id)) break;
        }
        std::map<string, int> setupMap = createMapFromVector(dataVector);
        if (parsedCurrentLine[0] != "0") {
            busStop.stop_id = stoi(parsedCurrentLine[setupMap["stop_id"]]);
            busStop.stop_code = stoi(parsedCurrentLine[setupMap["stop_code"]]);
            busStop.stop_name = parsedCurrentLine[setupMap["stop_name"]]; //
            busStop.stop_desc = parsedCurrentLine[setupMap["stop_desc"]]; //
            busStop.platform_code = (parsedCurrentLine[setupMap["platform_code"]] == "") ? 0 : stoi(parsedCurrentLine[setupMap["platform_code"]]);
            busStop.stop_lat = (parsedCurrentLine[setupMap["stop_lat"]] == "" || parsedCurrentLine[setupMap["stop_lat"]] == " ") ? 0 : std::stod(parsedCurrentLine[setupMap["stop_lat"]]);
            busStop.stop_lon = (parsedCurrentLine[setupMap["stop_lon"]] == "" || parsedCurrentLine[setupMap["stop_lon"]] == " ") ? 0 : std::stod(parsedCurrentLine[setupMap["stop_lon"]]);
            busStop.zone_id = (parsedCurrentLine[setupMap["zone_id"]] == "" || parsedCurrentLine[setupMap["zone_id"]] == " ") ? 0 : stoi(parsedCurrentLine[setupMap["zone_id"]]);
            busStop.stop_url = parsedCurrentLine[setupMap["stop_url"]]; //
            busStop.location_type = (parsedCurrentLine[setupMap["location_type"]] == "" || parsedCurrentLine[setupMap["location_type"]] == " " || setupMap["location_type"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["location_type"]]);
            busStop.parent_station = (parsedCurrentLine[setupMap["parent_station"]] == "" || parsedCurrentLine[setupMap["parent_station"]] == " " || setupMap["parent_station"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["parent_station"]]);
            busStop.stop_timezone = (parsedCurrentLine[setupMap["stop_timezone"]] == "" || parsedCurrentLine[setupMap["stop_timezone"]] == " " || setupMap["stop_timezone"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["stop_timezone"]]);
            busStop.wheelchair_boarding = (parsedCurrentLine[setupMap["wheelchair_boarding"]] == "" || parsedCurrentLine[setupMap["wheelchair_boarding"]] == " " || setupMap["wheelchair_boarding"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["wheelchair_boarding"]]);
            busStop.preferred = (parsedCurrentLine[setupMap["preferred"]] == "" || parsedCurrentLine[setupMap["preferred"]] == " " || setupMap["preferred"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["preferred"]]);
        } else {
            busStop.stop_id = 0;
            busStop.stop_name = "Does Not Exist";
        }
    } else if (type == code) {
        unsigned int lineNumber = 0;
        std::vector<string> dataVector;
        while (getline(stopFile, currentLine)) {
            lineNumber++;
            if (lineNumber == 1) [[unlikely]] {
                dataVector = parseDataCSV(currentLine);
                for (int i = 0; i < dataVector.size(); i++) {
                    cout << dataVector[i] << std::endl;
                }
            }

            parsedCurrentLine = parseDataCSV(currentLine);
            if (parsedCurrentLine[1] == std::to_string(busStop.stop_code)) break;
        }
        std::map<string, int> setupMap = createMapFromVector(dataVector);

        if (parsedCurrentLine[0] != "0") {
            busStop.stop_id = stoi(parsedCurrentLine[setupMap["stop_id"]]);
            busStop.stop_code = stoi(parsedCurrentLine[setupMap["stop_code"]]);
            busStop.stop_name = parsedCurrentLine[setupMap["stop_name"]]; //
            busStop.stop_desc = parsedCurrentLine[setupMap["stop_desc"]]; //
            busStop.platform_code = (parsedCurrentLine[setupMap["platform_code"]] == "") ? 0 : stoi(parsedCurrentLine[setupMap["platform_code"]]);
            busStop.stop_lat = (parsedCurrentLine[setupMap["stop_lat"]] == "" || parsedCurrentLine[setupMap["stop_lat"]] == " ") ? 0 : std::stod(parsedCurrentLine[setupMap["stop_lat"]]);
            busStop.stop_lon = (parsedCurrentLine[setupMap["stop_lon"]] == "" || parsedCurrentLine[setupMap["stop_lon"]] == " ") ? 0 : std::stod(parsedCurrentLine[setupMap["stop_lon"]]);
            busStop.zone_id = (parsedCurrentLine[setupMap["zone_id"]] == "" || parsedCurrentLine[setupMap["zone_id"]] == " ") ? 0 : stoi(parsedCurrentLine[setupMap["zone_id"]]);
            busStop.stop_url = parsedCurrentLine[setupMap["stop_url"]]; //
            busStop.location_type = (parsedCurrentLine[setupMap["location_type"]] == "" || parsedCurrentLine[setupMap["location_type"]] == " " || setupMap["location_type"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["location_type"]]);
            busStop.parent_station = (parsedCurrentLine[setupMap["parent_station"]] == "" || parsedCurrentLine[setupMap["parent_station"]] == " " || setupMap["parent_station"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["parent_station"]]);
            busStop.stop_timezone = (parsedCurrentLine[setupMap["stop_timezone"]] == "" || parsedCurrentLine[setupMap["stop_timezone"]] == " " || setupMap["stop_timezone"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["stop_timezone"]]);
            busStop.wheelchair_boarding = (parsedCurrentLine[setupMap["wheelchair_boarding"]] == "" || parsedCurrentLine[setupMap["wheelchair_boarding"]] == " " || setupMap["wheelchair_boarding"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["wheelchair_boarding"]]);
            busStop.preferred = (parsedCurrentLine[setupMap["preferred"]] == "" || parsedCurrentLine[setupMap["preferred"]] == " " || setupMap["preferred"] == 0) ? 0 : stoi(parsedCurrentLine[setupMap["preferred"]]);

        } else {
            busStop.stop_id = 0;
            busStop.stop_name = "Does Not Exist";
        }
    }
    
    stopFile.close();
    return busStop;
}

std::vector<tripSegment> getDayTimesAtStop(week day, const unsigned short int& id) {
    ifstream tripsFile(tripsPath);
    ifstream timeFile(stopTimesPath);

    string id_str = std::to_string(id);

    std::vector<tripSegment> output;
    std::vector<string> parsedData;

    std::vector<long> allTripIDs;

    std::map<string, int> refs;

    string currentLine;
    int lineNumber = 0;
    int stopNumberIndex = -1;

    while (getline(timeFile, currentLine)) {
        lineNumber++;
        if (lineNumber == 1) [[unlikely]] {
            parsedData = parseDataCSV(currentLine);
            for (int i = 0; i < parsedData.size(); i++) {
                refs[parsedData[i]] = i;
                if (parsedData[i] == "stop_id") { stopNumberIndex = i;}
            }
            break;
        }
        if (lineNumber == 2) {
            stopNumberIndex = 0;
            cout << "stop index not found";
            break;
        }
    }
    tripsFile.close();

    ifstream timeReviewFile(stopTimesPath);

    while (getline(timeReviewFile, currentLine)) {
        parsedData = parseDataCSV(currentLine);
        if (parsedData[stopNumberIndex] == id_str) {
            int tripID = stoi(parsedData[0]);
            allTripIDs.push_back(tripID);
        }
    }

    for (int i = 0; i < allTripIDs.size(); i++) {
        cout << allTripIDs[i] << " ";
    }

    return output;
}