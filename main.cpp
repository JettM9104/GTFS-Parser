#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

using std::cout, std::string, std::ifstream, std::ofstream, std::stoi;

typedef enum 
{ident, code}
stopType;

typedef enum {am, pm} tod;
typedef enum {mon, tue, wed, thu, fri, sat, sun} week;

const string root = "data/yrt_archive/";

const string stopPath = root + "stops.txt";
const string routePath = root + "routes.txt";
const string tripsPath = root + "trips.txt";
const string stopTimesPath = root + "stop_times.txt";
const string tripPath = root + "trips.txt";
const string calendarPath = root + "calendar.txt";
const string calendarDatesPath = root + "calendar_dates.txt";
const string agencyPath = root + "agency.txt";
const string shapePath = root + "shapes.txt";

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
    virtual string formattedTime() {
        return std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(s);
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

struct tripSegment { // params from stop_times plus routeid
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
    int route_id;

    void printInfo() {
        cout << "trip_id: " << trip_id << std::endl;
        cout << "arrival_time: " << arrival_time.formattedTime() << std::endl;
        cout << "departure_time: " << departure_time.formattedTime() << std::endl;
        cout << "route_id: " << route_id << std::endl;
    }
};

struct agency {
    string agency_id;
    string agency_name;
    string agency_url;
    string agency_timezone;
    string agency_lang;
    string agency_phone;
    string agency_fare_url;
    string agency_email;

    void printInfo() {
        cout << "agency_id: " << agency_id << std::endl;
        cout << "agency_name: " << agency_name << std::endl;
    }

    static void whatdidthefoxsay() {
        cout << "wowowowoowwowoowooooowowoowow\n";
    }
};

struct shape {
    int shape_id;
    double shape_pt_lat;
    double shape_pt_lon;
    int shape_pt_sequence;
    double shape_dist_traveled;

    shape(int id) {
        id = shape_id;
    }

    shape() = default;

    void printCoords() {
        cout << "shape_pt_lat: " << shape_pt_lat << std::endl;
        cout << "shape_pt_lon: " << shape_pt_lon << std::endl;
    }
};

struct trip {
    int route_id;
    string service_id;
    int trip_id;
    string trip_headsign;
    string trip_short_name;
    short direction_id;
    int block_id;
    int shape_id;
    short wheelchair_accessible;
    short bikes_allowed;
    short cars_allowed;

    bool resetAllValues(bool a) {
        if (!a) return 0;
        route_id = 0;
        service_id = "";
        trip_id = 0;
        trip_headsign = "";
        trip_short_name = "";
        direction_id = 0;
        block_id = 0;
        shape_id = 0;
        wheelchair_accessible = 0;
        bikes_allowed = 0;
        cars_allowed = 0;

        return 1;
    }
};

class bus {
public:
    string licencePlate;
    short int busID;
    busLine route;

    bus(busLine route) {
        this->route = route;
    }
    bus(short int busID) {
        this->busID = busID;
    }
    bus(busLine route, short int busID) {
        this->route = route;
        this->busID = busID;
    }
    bus() = default;


};

std::vector<string> parseDataCSV(const string& input);
std::unordered_map<string, int> createMapFromVector(std::vector<string> param);
time24 parseFormattedTime(string input);
bool isValid(int tripID, int year, int month, int date);
bool isValid(int tripID, week day); // more basic version; does not include calendar_dates.txt
week convertDateToWeek(int year, int month, int day);
time24 getCurrentTime();

std::vector<tripSegment> getDayTimesAtStop(int year, int month, int day, const unsigned short int id);
std::vector<tripSegment> getDayTimesAtStop(week day, const unsigned short int id);
std::vector<tripSegment> getRemainingDayStops(int year, int month, int day, const unsigned short int id, int hour, time24 time);
busLine getRouteInfo(const unsigned short int& id);
busLine getRouteInfo(const string& id);
stop getStopInfo(const unsigned short int& id, const stopType& type);
agency getAgencyInfo();
std::vector<shape> getShapeInfo(const int& shapeID);


int main(int argc, char* argv[]) {
    std::vector<tripSegment> times = getDayTimesAtStop(2025, 10, 10, 7114);

    for (int i = 0; i < times.size(); i++) {
        times[i].printInfo();
        cout << std::endl;
    }
}

week convertDateToWeek(int year, int month, int day) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }

    int K = year % 100;
    int J = year / 100;

    int h = (day + 13*(month + 1)/5 + K + K/4 + J/4 + 5*J) % 7;

    int dayIndex;
    switch(h) {
        case 0: dayIndex = sat; break;
        case 1: dayIndex = sun; break;
        case 2: dayIndex = mon; break;
        case 3: dayIndex = tue; break;
        case 4: dayIndex = wed; break;
        case 5: dayIndex = thu; break;
        case 6: dayIndex = fri; break;
    }

    return static_cast<week>(dayIndex);
}

bool isValid(int tripID, int year, int month, int date) {
    week day = convertDateToWeek(year, month, date);
    ifstream trip(tripPath);
    string currentLine;

    std::vector<string> parsedLine;

    string serviceID;

    string tidString = std::to_string(tripID);

    while (getline(trip, currentLine)) {
        parsedLine = parseDataCSV(currentLine);
        if (parsedLine[2] == tidString) {
            serviceID = parsedLine[1];
            break;
        }
    }

    trip.close();

    ifstream calendar(calendarPath);
    bool found = false;

    while (getline(calendar, currentLine)) {
        parsedLine = parseDataCSV(currentLine);

        if (parsedLine[0] == serviceID) {
            found = true;
            break;
        }
    }
    cout << "yay\n";
    if (found) {
        cout << "found\n";
        ifstream dates(calendarPath);

        string combined = std::to_string(year);

        string mo = std::to_string(month);

        string da = std::to_string(date);

        if (mo.length() <= 1) mo = "0" + mo;
        if (da.length() <= 1) mo = "0" + da;

        combined += mo;
        combined += da;

        cout << combined << std::endl;

        while (getline(dates, currentLine)) {
            parsedLine = parseDataCSV(currentLine);
            if (parsedLine[0] == serviceID) break;
        }
        cout << "b4 switch\n" << parsedLine[1] << std::endl << std::endl;
        switch (day) {
            case mon:
                if (stoi(parsedLine[1]) == 1) return true;
                else return false;
                break;
            
            case tue:
                if (stoi(parsedLine[2]) == 1) return true;
                else return false;
                break;
            
            case wed:
                if (stoi(parsedLine[3]) == 1) return true;
                else return false;
                break;
            
            case thu:
                if (stoi(parsedLine[4]) == 1) return true;
                else return false;
                break;

            case fri:
                if (stoi(parsedLine[5]) == 1) return true;
                else return false;
                break;

            case sat:
                if (stoi(parsedLine[6]) == 1) return true;
                else return false;
                break;
            
            case sun:
                if (stoi(parsedLine[7]) == 1) return true;
                else return false;
                break;
            
            default:
                cout << "what kind of sorcery is this bro";
                return false;
            
        }
        cout << "after switch\n";

        calendar.close();
    }
    else {
        cout << "this is special :O";
        ifstream dates(calendarDatesPath);

        string combined = std::to_string(year);

        string mo = std::to_string(month);

        string da = std::to_string(date);

        if (mo.length() <= 1) mo = "0" + mo;
        if (da.length() <= 1) mo = "0" + da;

        combined += mo;
        combined += da;

        while (getline(dates, currentLine)) {
            parsedLine = parseDataCSV(currentLine);
            if (parsedLine[1] == serviceID && parsedLine[2] == "1") return true;
        }
    }
    return false;
}

bool isValid(int tripID, week day) {
    ifstream trip(tripPath);
    string currentLine;

    std::vector<string> parsedLine;

    string serviceID;

    string tidString = std::to_string(tripID);

    while (getline(trip, currentLine)) {
        parsedLine = parseDataCSV(currentLine);
        if (parsedLine[2] == tidString) {
            serviceID = parsedLine[1];
            break;
        }
    }

    trip.close();

    ifstream calendar(calendarPath);
    bool found = false;

    while (getline(calendar, currentLine)) {
        parsedLine = parseDataCSV(currentLine);

        if (parsedLine[0] == serviceID) {
            found = true;
            break;
        }
    }
    if (found) {
        switch (day) {
            case mon:
                if (stoi(parsedLine[1]) == 1) return true;
                else return false;
                break;
            
            case tue:
                if (stoi(parsedLine[2]) == 1) return true;
                else return false;
                break;
            
            case wed:
                if (stoi(parsedLine[3]) == 1) return true;
                else return false;
                break;
            
            case thu:
                if (stoi(parsedLine[4]) == 1) return true;
                else return false;
                break;

            case fri:
                if (stoi(parsedLine[5]) == 1) return true;
                else return false;
                break;

            case sat:
                if (stoi(parsedLine[6]) == 1) return true;
                else return false;
                break;
            
            case sun:
                if (stoi(parsedLine[7]) == 1) return true;
                else return false;
                break;
            
            default:
                cout << "what kind of sorcery is this bro";
                return false;
            
        }

        calendar.close();
    }
    return 0;
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

std::unordered_map<string, int> createMapFromVector(std::vector<string> param) {
    int length = param.size();
    std::unordered_map<string, int> output;
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
        std::unordered_map<string, int> setupMap = createMapFromVector(dataVector);
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
        std::unordered_map<string, int> setupMap = createMapFromVector(dataVector);

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

std::vector<tripSegment> getDayTimesAtStop(int year, int month, int day, const unsigned short int id) {
    ifstream timeFile(stopTimesPath);

    string id_str = std::to_string(id);

    std::vector<tripSegment> output;
    std::vector<string> parsedData;

    std::vector<long> allTripIDs;

    std::unordered_map<string, int> refs;

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
    timeFile.close();

    ifstream timeReviewFile(stopTimesPath);

    while (getline(timeReviewFile, currentLine)) {
        parsedData = parseDataCSV(currentLine);
        if (parsedData[stopNumberIndex] == id_str) {
            int tripID = stoi(parsedData[0]);
            allTripIDs.push_back(tripID);

            tripSegment localVar;

            // required fields
            localVar.trip_id = tripID;
            localVar.arrival_time = parseFormattedTime(parsedData[1]);
            localVar.departure_time = parseFormattedTime(parsedData[2]);
            localVar.stop_id = stoi(parsedData[3]);
            localVar.stop_sequence = stoi(parsedData[4]);

            localVar.stop_headsign = ((refs["stop_headsign"] != 0) && (parsedData[refs["stop_headsign"]] != "" && parsedData[refs["stop_headsign"]] != " ")) ? stoi(parsedData[refs["stop_headsign"]]) : 0;
            localVar.pickup_type = ((refs["pickup_type"] != 0) && (parsedData[refs["pickup_type"]] != "" && parsedData[refs["pickup_type"]] != " ")) ? stoi(parsedData[refs["pickup_type"]]) : 0;
            localVar.drop_off_type = ((refs["drop_off_type"] != 0) && (parsedData[refs["drop_off_type"]] != "" && parsedData[refs["drop_off_type"]] != " ")) ? stoi(parsedData[refs["drop_off_type"]]) : 0;
            localVar.shape_dist_traveled = ((refs["shape_dist_traveled"] != 0) && (parsedData[refs["shape_dist_traveled"]] != "" && parsedData[refs["shape_dist_traveled"]] != " ")) ? stoi(parsedData[refs["shape_dist_traveled"]]) : 0;
            localVar.timepoint = ((refs["timepoint"] != 0) && (parsedData[refs["timepoint"]] != "" && parsedData[refs["timepoint"]] != " ")) ? stoi(parsedData[refs["timepoint"]]) : 0;

            output.push_back(localVar);
        }
    }
    lineNumber = 0;
    timeReviewFile.close();

    ifstream tripsFile(tripPath);

    while (getline(tripsFile, currentLine)) {
        lineNumber++;
        parsedData = parseDataCSV(currentLine);
        if (lineNumber == 1) {
            refs.clear();
            for (int i = 0; i < parsedData.size(); i++) {
                refs[parsedData[i]] = i;
            }
        }
        for (int i = 0; i < output.size(); i++) {
            if (std::to_string(output[i].trip_id) == parsedData[refs["trip_id"]]) {
                output[i].route_id = stoi(parsedData[refs["route_id"]]);
            }
        }        
        
    }

    for (int i = 0; i < output.size(); i++) {
        if (!isValid(output[i].trip_id, year, month, day)) {
            output.erase(output.begin() + i);
        }
    }
    tripsFile.close();
    

    return output;
}

std::vector<tripSegment> getDayTimesAtStop(week day, const unsigned short int id) {
    ifstream tripsFile(tripsPath);
    ifstream timeFile(stopTimesPath);

    string id_str = std::to_string(id);

    std::vector<tripSegment> output;
    std::vector<string> parsedData;

    std::vector<long> allTripIDs;

    std::unordered_map<string, int> refs;

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

            tripSegment localVar;

            // required fields
            localVar.trip_id = tripID;
            localVar.arrival_time = parseFormattedTime(parsedData[1]);
            localVar.departure_time = parseFormattedTime(parsedData[2]);
            localVar.stop_id = stoi(parsedData[3]);
            localVar.stop_sequence = stoi(parsedData[4]);

            localVar.stop_headsign = ((refs["stop_headsign"] != 0) && (parsedData[refs["stop_headsign"]] != "" && parsedData[refs["stop_headsign"]] != " ")) ? stoi(parsedData[refs["stop_headsign"]]) : 0;
            localVar.pickup_type = ((refs["pickup_type"] != 0) && (parsedData[refs["pickup_type"]] != "" && parsedData[refs["pickup_type"]] != " ")) ? stoi(parsedData[refs["pickup_type"]]) : 0;
            localVar.drop_off_type = ((refs["drop_off_type"] != 0) && (parsedData[refs["drop_off_type"]] != "" && parsedData[refs["drop_off_type"]] != " ")) ? stoi(parsedData[refs["drop_off_type"]]) : 0;
            localVar.shape_dist_traveled = ((refs["shape_dist_traveled"] != 0) && (parsedData[refs["shape_dist_traveled"]] != "" && parsedData[refs["shape_dist_traveled"]] != " ")) ? stoi(parsedData[refs["shape_dist_traveled"]]) : 0;
            localVar.timepoint = ((refs["timepoint"] != 0) && (parsedData[refs["timepoint"]] != "" && parsedData[refs["timepoint"]] != " ")) ? stoi(parsedData[refs["timepoint"]]) : 0;

            output.push_back(localVar);
        }
    }
    timeReviewFile.close();

    return output;
}

agency getAgencyInfo() {
    agency output;
    ifstream agencyFile(agencyPath);
    string line;
    std::vector<string> parsedLine;
    std::unordered_map<string, int> refs;
    short lineNumber = 0;

    while (getline(agencyFile, line)) {
        lineNumber++;
        parsedLine = parseDataCSV(line);
        if (lineNumber == 1) {
            for (int i = 0; i < parsedLine.size(); i++) {
                refs[parsedLine[i]] = i;
            }
        } else if (lineNumber == 2) {
            output.agency_id = parsedLine[0];
            output.agency_name = parsedLine[1];
            output.agency_url = parsedLine[2];
            output.agency_timezone = parsedLine[3];
            output.agency_lang = (refs["agency_lang"] == 0 || parsedLine[refs["agency_lang"]] == "" || parsedLine[refs["agency_lang"]] == " ") ? "undef" : parsedLine[refs["agency_lang"]];
            output.agency_phone = (refs["agency_phone"] == 0 || parsedLine[refs["agency_phone"]] == "" || parsedLine[refs["agency_phone"]] == " ") ? "undef" : parsedLine[refs["agency_phone"]];
            output.agency_fare_url = (refs["agency_fare_url"] == 0 || parsedLine[refs["agency_fare_url"]] == "" || parsedLine[refs["agency_fare_url"]] == " ") ? "undef" : parsedLine[refs["agency_fare_url"]];
            output.agency_email = (refs["agency_email"] == 0 || parsedLine[refs["agency_email"]] == "" || parsedLine[refs["agency_email"]] == " ") ? "undef" : parsedLine[refs["agency_email"]];
        } else {
            cout << "unavaliavle";
        }
        
    }
    agencyFile.close();
    return output;
}

trip getTripInfo(const int& id) {
    ifstream tripFile(tripPath);
    string currentLine;
    std::vector<string> parsedCurrentLine;
    trip output;
    output.trip_id = id;
    std::unordered_map<string, int> refs;
    int lineNumber = 0;
    bool found;

    string idStr = std::to_string(id);

    while (getline(tripFile, currentLine)) {
        lineNumber++;
        parsedCurrentLine = parseDataCSV(currentLine);

        if (lineNumber == 1) [[unlikely]] {
            for (int i = 0; i < parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
            continue;
        }

        if (parsedCurrentLine[refs["trip_id"]] == idStr) {
            output.route_id = stoi(parsedCurrentLine[refs["route_id"]]);
            output.service_id = parsedCurrentLine[refs["service_id"]];
            output.trip_id = stoi(parsedCurrentLine[refs["trip_id"]]);
            output.trip_headsign = (parsedCurrentLine[refs["trip_id"]] != "" && parsedCurrentLine[refs["trip_id"]] != " " && refs["trip_id"] != 0) ? (parsedCurrentLine[refs["trip_id"]]) : (""); // for strings
            output.trip_short_name = (parsedCurrentLine[refs["trip_short_name"]] != "" && parsedCurrentLine[refs["trip_short_name"]] != " " && refs["trip_short_name"] != 0) ? (parsedCurrentLine[refs["trip_short_name"]]) : (""); // for strings
            output.direction_id = (parsedCurrentLine[refs["direction_id"]] != "" && parsedCurrentLine[refs["direction_id"]] != " " && refs["direction_id"] != 0) ? (stoi(parsedCurrentLine[refs["direction_id"]])) : (0); // for integers
            output.block_id = (parsedCurrentLine[refs["block_id"]] != "" && parsedCurrentLine[refs["block_id"]] != " " && refs["block_id"] != 0) ? (stoi(parsedCurrentLine[refs["block_id"]])) : (0); // for integers
            output.shape_id = (parsedCurrentLine[refs["shape_id"]] != "" && parsedCurrentLine[refs["shape_id"]] != " " && refs["shape_id"] != 0) ? (stoi(parsedCurrentLine[refs["shape_id"]])) : (0); // for integers
            output.wheelchair_accessible = (parsedCurrentLine[refs["wheelchair_accessible"]] != "" && parsedCurrentLine[refs["wheelchair_accessible"]] != " " && refs["wheelchair_accessible"] != 0) ? (stoi(parsedCurrentLine[refs["wheelchair_accessible"]])) : (0); // for integers
            output.bikes_allowed = (parsedCurrentLine[refs["bikes_allowed"]] != "" && parsedCurrentLine[refs["bikes_allowed"]] != " " && refs["bikes_allowed"] != 0) ? (stoi(parsedCurrentLine[refs["bikes_allowed"]])) : (0); // for integers
            output.cars_allowed = (parsedCurrentLine[refs["cars_allowed"]] != "" && parsedCurrentLine[refs["cars_allowed"]] != " " && refs["cars_allowed"] != 0) ? (stoi(parsedCurrentLine[refs["cars_allowed"]])) : (0); // for integers
            found = true;
            break;
        }
    }
    if (!found) output.resetAllValues(1);
    tripFile.close();
    return output;
}

std::vector<shape> getShapeInfo(const int& shapeID) {
    ifstream shapeFile(shapePath);

    std::vector<shape> output(shapeID);

    string currentLine;
    std::vector<string> parsedCurrentLine;
    std::unordered_map<string, int> refs;

    int lineNumber = 0;

    while (getline(shapeFile, currentLine)) {
        lineNumber++;
        parsedCurrentLine = parseDataCSV(currentLine);
        if (lineNumber == 1) [[unlikely]] {
            for (int i = 0; i < parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
        }
        else {
            shape localShape;

            localShape.shape_id = stoi(parsedCurrentLine[refs["shape_id"]]);
            localShape.shape_pt_lat = std::stod(parsedCurrentLine[refs["shape_pt_lat"]]);
            localShape.shape_pt_lon = std::stod(parsedCurrentLine[refs["shape_pt_lon"]]);
            localShape.shape_pt_sequence = stoi(parsedCurrentLine[refs["shape_pt_sequence"]]);

            if (refs["shape_dist_traveled"] != 0 && parsedCurrentLine[refs["shape_dist_traveled"]] != "" && parsedCurrentLine[refs["shape_dist_traveled"]] != " ") {
                localShape.shape_pt_sequence = stoi(parsedCurrentLine[refs["shape_dist_traveled"]]);
            }

            output.push_back(localShape);
        }

    }
    shapeFile.close();
    return output;
}
