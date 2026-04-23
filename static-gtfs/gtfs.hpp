
#ifndef GTFS_HPP
#define GTFS_HPP

#pragma region LIBRARIES AND NAMESPACES

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <ctime>
#include <array>
#include <algorithm>
#include <stdlib.h>
#include <iomanip>
#include <cmath>


using std::cout, std::string, std::ifstream, std::ofstream, std::cerr, std::to_string, std::endl;

#pragma endregion

namespace gtfs { // MARK: BEGINNING OF NAMESPACE GTFS

#pragma region ENUMS AND VARIABLES    

typedef enum {mon = 0, tue, wed, thu, fri, sat, sun} week;
typedef enum {in_use = 0, expired = -1, upcoming = 1, no_result = 10, no_result_a = 11, no_result_b = 12} feedStatus;

float π = 3.14159;

typedef unsigned long long int αβγδεζηθικλμνξοπρστυφχψω; // little easter egg :D

string path = "/data/yrt_archive/";

string root = "/Users/jettmu/Documents/VSCode/GTFS Parser/static-gtfs" + path;

string stopPath = root + "stops.txt";
string routePath = root + "routes.txt";
string tripsPath = root + "trips.txt";
string stopTimesPath = root + "stop_times.txt";
string tripPath = root + "trips.txt";
string calendarPath = root + "calendar.txt";
string calendarDatesPath = root + "calendar_dates.txt";
string agencyPath = root + "agency.txt";
string shapePath = root + "shapes.txt";
string feedInfoFile = root + "feed_info.txt";

const int precision = 8;
const int defPrecision = 6;

#pragma endregion ENUMS AND VARIABLES
#pragma region CLASSES AND STRUCTS

class time {
public:
    int h, m;
    float s;

    string formattedTime() {
        return std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(s);
    }

    string roundedTime() {
        return std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(static_cast<int>(round(s)));
    }

    string leadingRoundedTime() {
        return (std::to_string(h).length() < 2 ? "0" + std::to_string(h) : std::to_string(h)) + ":" + 
               (std::to_string(m).length() < 2 ? "0" + std::to_string(m) : std::to_string(m)) + ":" + 
               (std::to_string((int)std::round(s)).length() < 2 ? "0" + std::to_string((int)std::round(s)) : std::to_string((int)std::round(s)));
    }

    time() = default;
    time(int h, int m, int s) {
        this->h = h;
        this->m = m;
        this->s = s;
    }

    virtual bool operator>(time other) {
        if (this->h < other.h) {
            return false;
        } else if (this->h > other.h) {
            return true;
        } else if (this->m < other.m) {
            return false;
        } else if (this->m > other.m) {
            return true;
        } else if (this->s < other.s) {
            return false;
        } else if (this->s > other.s) {
            return true;
        } else {
            return false;
        }
    }
    
    virtual bool operator<(time other) {
        if (this->h > other.h) {
            return false;
        } else if (this->h < other.h) {
            return true;
        } else if (this->m > other.m) {
            return false;
        } else if (this->m < other.m) {
            return true;
        } else if (this->s > other.s) {
            return false;
        } else if (this->s < other.s) {
            return true;
        } else {
            return false;
        }
    }

    virtual bool operator<=(time other) {
        if (this->h > other.h) {
            return false;
        } else if (this->h < other.h) {
            return true;
        } else if (this->m > other.m) {
            return false;
        } else if (this->m < other.m) {
            return true;
        } else if (this->s > other.s) {
            return false;
        } else if (this->s < other.s) {
            return true;
        } else {
            return true;
        }
    }

    virtual bool operator>=(time other) {
        if (this->h < other.h) {
            return false;
        } else if (this->h > other.h) {
            return true;
        } else if (this->m < other.m) {
            return false;
        } else if (this->m > other.m) {
            return true;
        } else if (this->s < other.s) {
            return false;
        } else if (this->s > other.s) {
            return true;
        } else {
            return true;
        }
    }

    virtual bool operator==(time other) {
        return (other.h == this->h && other.m == this->m && other.s == this->s);
    }

    virtual bool operator!=(time other) {
        return !(other.h == this->h && other.m == this->m && other.s == this->s);
    }

    virtual time operator+(time other) { // cannot overlap, note that during calling
        time output;

        output.h = this->h + other.h;
        output.m = this->m + other.m;
        output.s = this->s + other.s;
        if (output.s >= 60) {
            output.m += 1;
            output.s -= 60;
        }
        if (output.m >= 60) {
            output.h += 1;
            output.m -= 1;
        }
        if (output.h >= 24) output.h -= 24;

        return output;
    }
};
class calendar_day { // to only be used in structs and functions that truly need it, or, in suitable overloads
public:
    int year;
    int month;
    int day;

    calendar_day() = default;
    calendar_day(int year, int month, int day) {
        this->year = year;
        this->month = month;
        this->day = day;
    }

    void printInfo() {
        cout << "year: " << year << std::endl;
        cout << "month: " << month << std::endl;
        cout << "day: " << day << std::endl;
    }

    bool operator< (const calendar_day& other) {
        if (other.year > this->year) {
            return true;
        } else if (other.year < this->year) {
            return false;
        } else { // year is equal
            if (other.month > this->month) {
                return true;
            } else if (other.month < this->month) {
                return false;
            } else { // months are equal
                if (other.day > this->day) {
                    return true;
                } else if (other.day > this->day) {
                    return false;
                } else { // days are equal
                    return false;
                }
            }
        }
    }

    bool operator> (const calendar_day& other) {
        if (other.year < this->year) {
            return true;
        } else if (other.year > this->year) {
            return false;
        } else { // year is equal
            if (other.month < this->month) {
                return true;
            } else if (other.month > this->month) {
                return false;
            } else { // months are equal
                if (other.day < this->day) {
                    return true;
                } else if (other.day > this->day) {
                    return false;
                } else { // days are equal
                    return false;
                }
            }
        }
    }

    bool operator<= (const calendar_day& other) {
        if (other.year > this->year) {
            return true;
        } else if (other.year < this->year) {
            return false;
        } else { // year is equal
            if (other.month > this->month) {
                return true;
            } else if (other.month < this->month) {
                return false;
            } else { // months are equal
                if (other.day > this->day) {
                    return true;
                } else if (other.day > this->day) {
                    return false;
                } else { // days are equal
                    return true;
                }
            }
        }
    }

    bool operator>= (const calendar_day& other) {
        if (other.year < this->year) {
            return true;
        } else if (other.year > this->year) {
            return false;
        } else { // year is equal
            if (other.month < this->month) {
                return true;
            } else if (other.month > this->month) {
                return false;
            } else { // months are equal
                if (other.day < this->day) {
                    return true;
                } else if (other.day > this->day) {
                    return false;
                } else { // days are equal
                    return true;
                }
            }
        }
    }

    bool operator== (const calendar_day& other) {
        return (other.year == this->year && other.month == this->month && other.day == this->day);
    }

    calendar_day operator+ (const calendar_day& other) {
        calendar_day output;

        int regularDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int leapDays[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        output.year = this->year + other.year;
        output.month = this->month + other.month;
        output.day = this->day + other.day;

        if (output.month > 12) {
            output.year += 1;
            output.month -= 12;
        }

        if ((output.year % 4 == 0 && output.year % 100 != 0 )|| output.year % 400 == 0) {
            if (output.day > leapDays[output.month - 1]) {
                output.day -= leapDays[output.month - 1];
                output.month++;
            }
        } else {
            if (output.day > regularDays[output.month - 1]) {
                output.day -= regularDays[output.month - 1];
                output.month++;
            }
        }
        return output;
    }
};

struct intstr {
    int num;
    string str;

    intstr() = default;
    intstr(int a, string b) : num(a), str(std::move(b)) {}
    intstr(string a, int b) : num(b), str(std::move(a)) {}
    intstr(int a)           : num(a) {}
    intstr(string a)        : str(std::move(a)) {}
};
struct matchsearch {
    intstr text;
    int score;
};

struct route {
    enum continuous_pickup_dropoff {continuous = 0, no_continuous = 1, phone_agency = 2, driver_coordinate = 3};
    enum cemv {no_info = 0, with_cemv = 1, no_cemv = 2}; // cemv support in route will take precedence
    enum type {light_rail = 0, underground = 1, rail = 2, bus = 3, ferry = 4, cable_tram = 5, aerial_lift = 6, funicular = 7, trolleybus = 11, monorail = 12};

    string route_id;
    string agency_id;
    string route_short_name;
    string route_long_name;
    string route_desc;
    type route_type;
    string route_url;
    string route_color;
    string route_text_color;

    unsigned int route_sort_order;
    continuous_pickup_dropoff continuous_pickup;
    continuous_pickup_dropoff continuous_drop_off;

    string network_id;
    cemv cemv_support;
};
struct stop {
    enum location {stop_platform = 0, station = 1, entrance_exit = 2, generic_node = 3, boarding_area = 4};
    enum wheelchair {no_info = 0, wheelcahir_boarding = 1, no_wheelchair_boarding = 2}; // 0 can also represent "inheret from parent station"
    enum access {no_street_access = 0, direct_access = 1};

    string stop_id;
    string stop_code;
    string stop_name;
    string tts_stop_name;
    string stop_desc;

    double stop_lat;
    double stop_lon;

    string zone_id;
    string stop_url;
    
    location location_type;
    string parent_station; // refrences antoher stop.stop_id
    string stop_timezone;
    wheelchair wheelchair_boarding;
    string level_id;
    string platform_code;

    access stop_access;
};
struct stop_time {
    enum pickup_dropoff {regular = 0, not_avaliable = 1, phone_agency = 2, driver_coordinate = 3};
    enum continuous_pickup_dropoff {continuous = 0, no_continuous = 1, phone_agency_continuous = 2, driver_coordiante_continuous = 3};
    enum timepoint_type {approximate = 0, exact = 1};

    string trip_id;
    time arrival_time;
    time departure_time;
    string stop_id;
    string location_group_id;
    string location_id;

    unsigned int stop_sequence;

    string stop_headsign;

    time start_pickup_drop_off_window;
    time end_pickup_drop_off_window;

    pickup_dropoff pickup_type;
    pickup_dropoff drop_off_type;

    continuous_pickup_dropoff continuous_pickup;
    continuous_pickup_dropoff continuous_drop_off;

    float shape_dist_traveled;

    timepoint_type timepoint;

    string pickup_booking_rule_id;
    string drop_off_booking_rule_id;
};
struct agency {
    enum cemv {no_info = 0, with_cemv = 1, no_cemv = 2}; // cemv support in route will take precedence

    string agency_id;
    string agency_name;
    string agency_url;
    string agency_timezone;
    string agency_lang;
    string agency_phone;
    string agency_fare_url;
    string agency_email;
    cemv cemv_support;
};
struct shape {
    string shape_id;
    double shape_pt_lat;
    double shape_pt_lon;
    unsigned int shape_pt_sequence;
    unsigned int shape_dist_traveled;
};
struct trip {
    enum allowable {no_info = 0, allowed = 1, not_allowed = 2};

    string route_id;
    string service_id;
    string trip_id;
    string trip_headsign;
    string trip_short_name;
    bool direction_id;
    string block_id;
    string shape_id;

    allowable wheelchair_accessible;
    allowable bikes_allowed;
    allowable cars_allowed;
};
struct calendar {
    string service_id;

    bool monday, tuesday, wednesday, thursday, friday, saturday, sunday;

    calendar_day start_date;
    calendar_day end_date;
};
struct calendar_date {
    enum exception {added = 1, removed = 2};
    string service_id;
    calendar_day date;
    exception exception_type;
};

struct trip_segment {
    stop_time stop;
    string route_id;
};
struct service {
    calendar schedule;
    std::vector<calendar_date> exceptions;
};

#pragma endregion CLASSES AND STRUCTS
#pragma region HELPER FUNCTIONS

int to_integer(string input) {
    if (input == "" || input == " ") return 0;

    return std::stoi(input);
}
double to_double(string input) {
    if (input == "" || input == " ") return 0;

    return std::stod(input);
}
float to_float(string input) {
    if (input == "" || input == " ") return 0;

    return std::stof(input);
}

double getDistanceKM(double lat1, double lon1, double lat2, double lon2) {
    const double earth_radius = 6371.0;

    double lat1_rad = lat1 * 3.1415 / 180.0;
    double lon1_rad = lon1 * 3.1415 / 180.0;
    double lat2_rad = lat2 * 3.1415 / 180.0;
    double lon2_rad = lon2 * 3.1415 / 180.0;

    double delta_lat = lat2_rad - lat1_rad;
    double delta_lon = lon2_rad - lon1_rad;

    double a = sin(delta_lat / 2) * sin(delta_lat / 2) + 
                cos(lat1_rad) * cos(lat2_rad) *
                sin(delta_lon / 2) * sin(delta_lon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return earth_radius * c;
}
double dist(double a, double b, double c, double d) {
    return sqrt((a-c) * (a-c) + (b-d) * (b-d));
}
int levenshtein(const string &a, const string &b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1));

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            dp[i][j] = std::min({ 
                dp[i-1][j] + 1,     // delete
                dp[i][j-1] + 1,     // insert
                dp[i-1][j-1] + cost // replace
            });
        }
    }

    return dp[m][n];
}
calendar_day parseFormattedDate(string input) {
    calendar_day output;

    output.year = stoi(input.substr(0, 4));
    output.month = stoi(input.substr(4, 2));
    output.day = stoi(input.substr(6, 2));

    return output;
}
calendar_day getToday() {
    calendar_day output;
    std::time_t rn = std::time(0);
    std::tm* ltm = std::localtime(&rn);

    output.year = 1900 + ltm->tm_year;
    output.month = 1 + ltm->tm_mon;
    output.day = ltm->tm_mday;

    return output;
}
time getCurrentTime() {
    time output;
    std::time_t currentTime = std::time(nullptr);

    std::tm* localTime = std::localtime(&currentTime);

    output.h = localTime->tm_hour;   // Hour (0-23)
    output.m = localTime->tm_min; // Minute (0-59)
    output.s = localTime->tm_sec; // Second (0-59)

    return output;
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
        default: cout << "exit conditino"; break;
    }
    return static_cast<week>(dayIndex);
}
time parseFormattedTime(string input) {
    if (input == "" || input == " ") return time(-1, -1, -1);

    time output;
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

    for (char c : input) {
        if (c == ',') {
            output.push_back(additions);
            additions.clear();
        } else if (c != '\r') {
            additions += c;
        }
    }

    output.push_back(additions);

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
double getScore(string input) {
    std::vector<int> weights = {1};
    double total = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] >= 'A' && input[i] <= 'Z') {
            input[i] = input[i] + 32;
        }
    }
    for (int i = 0; i < input.length(); i++) {
        total += static_cast<double>(input[i]) * static_cast<double>(weights[i % weights.size()]) - 48.0;
    }

    total /= input.length();
    return total;
}

#pragma endregion HELPER FUNCTIONS
#pragma region FUNCTIONS

trip getTripInfo(string trip_id) { // requirements: trips.txt
    trip output;

    ifstream tripFile(tripPath);
    string currentLine;
    std::vector<string> parsedCurrentLine;
    std::unordered_map<string, int> refs;
    bool firstLine = true;

    while (getline(tripFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            for (int i = 0; i < parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["trip_id"]] == trip_id) {
            // required fields
            output.route_id = parsedCurrentLine[refs["route_id"]];
            output.service_id = parsedCurrentLine[refs["service_id"]];
            output.trip_id = trip_id;

            // optional/conditionally required/conditionally forbidden fields
            { auto find = refs.find("trip_headsign");
            if (find != refs.end()) output.trip_headsign = parsedCurrentLine[find->second]; }

            { auto find = refs.find("trip_short_name");
            if (find != refs.end()) output.trip_short_name = parsedCurrentLine[find->second]; }

            { auto find = refs.find("block_id");
            if (find != refs.end()) output.block_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("shape_id");
            if (find != refs.end()) output.shape_id = parsedCurrentLine[find->second]; }

            // bool field
            { auto find = refs.find("direction_id");
            if (find != refs.end()) output.direction_id = static_cast<bool>(stoi(parsedCurrentLine[find->second])); }

            // allowable fields
            { auto find = refs.find("wheelchair_accessible");
            if (find != refs.end()) output.wheelchair_accessible = static_cast<trip::allowable>(stoi(parsedCurrentLine[find->second])); }

            { auto find = refs.find("bikes_allowed");
            if (find != refs.end()) output.bikes_allowed = static_cast<trip::allowable>(stoi(parsedCurrentLine[find->second])); }

            { auto find = refs.find("cars_allowed");
            if (find != refs.end()) output.cars_allowed = static_cast<trip::allowable>(stoi(parsedCurrentLine[find->second])); }

            break; // assume trip_id only shows up once in a trips.txt 
        }
    }


    tripFile.close();
    return output;
}
bool isTripValid(string trip_id, int year, int month, int day, bool noException = false) { // requirements: trips.txt and calendar.txt, and if noException is false, calendar_dates.txt
    bool output = false;

    string constructedDate = to_string(year) + 
                            (month < 10 ? "0" + to_string(month) : to_string(month)) + 
                            (day < 10 ? "0" + to_string(day) : to_string(day));
                        
    week dayOfWeek = convertDateToWeek(year, month, day);
    string service_id = getTripInfo(trip_id).service_id;

    ifstream calendarFile = ifstream(calendarPath);
    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool firstLine = true;
    std::unordered_map<string, int> refs;

    while (getline(calendarFile, currentLine)) { // to refrence calendar.txt
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            for (int i = 0; i < parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
            firstLine = false;
            continue;
        }
        if (parsedCurrentLine[refs["service_id"]] == service_id) {
            string test;

            switch (static_cast<int>(dayOfWeek)) {
                case 0:
                    test = "monday";
                    break;
                
                case 1:
                    test = "tuesday";
                    break;
                
                case 2:
                    test = "wednesday";
                    break;

                case 3:
                    test = "thursday";
                    break;

                case 4:
                    test = "friday";
                    break;

                case 5:
                    test = "saturday";
                    break;
                
                case 6:
                    test = "sunday";
                    break;
                
                default:
                    cerr << "dayOfWeek is invalid or unintalized\n";
                    return 0;
            }
            if (parseFormattedDate(parsedCurrentLine[refs["start_date"]]) > calendar_day(year, month, day) 
                || parseFormattedDate(parsedCurrentLine[refs["end_date"]]) < calendar_day(year, month, day)) {
                
                output = false;
                break;
            }
            if (static_cast<bool>(to_integer(parsedCurrentLine[refs[test]]))) {
                output = true;
            }
            break;
        }
    }
    calendarFile.close();

    ifstream calendarDatesFile = ifstream(calendarDatesPath);

    if (noException) return output;

    firstLine = true;
    currentLine = "";
    parsedCurrentLine = std::vector<string>(0);
    refs.clear();



    while (getline(calendarDatesFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            for (int i = 0; i < parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["service_id"]] == service_id && parsedCurrentLine[refs["date"]] == constructedDate) {
            if (parsedCurrentLine[refs["exception_type"]] == "1") output = true;
            else output = false;
        }
        
    }

    calendarDatesFile.close();
    
    return output;
}
bool isTripValid(string trip_id, calendar_day date, bool noException = false) { // requirements: trips.txt and calendar.txt, and if noException is false, calendar_dates.txt
    return isTripValid(trip_id, date.year, date.month, date.day, noException);
}
feedStatus verifyGTFS(int year, int month, int day) { // requirements: feed_info.txt
    ifstream feedInfo(feedInfoFile);
    string currentLine;
    std::vector<string> parsedCurrentLine;

    int lineNumber = 0;
    
    std::unordered_map<string, int> refs;

    calendar_day inputDate(year, month, day);

    while (getline(feedInfo, currentLine)) {
        lineNumber++;

        parsedCurrentLine = parseDataCSV(currentLine);

        if (lineNumber == 1) {
            for (int i = 0; i < parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
        }

        if (lineNumber == 2) {
            calendar_day x;
            calendar_day y;

            if (refs.find("feed_end_date") == refs.end()) return no_result_a; 
            else x = parseFormattedDate(parsedCurrentLine[refs["feed_end_date"]]);

            if (refs.find("feed_start_date") == refs.end()) return no_result_b;
            else y = parseFormattedDate(parsedCurrentLine[refs["feed_start_date"]]);

            if (x >= inputDate && y <= inputDate) return in_use;
            else if (x <= inputDate) return expired; // expired
            else return upcoming; // upcoming
            
            break;
        }

    }
    return no_result;
}
feedStatus verifyGTFS(calendar_day date) { // requirements: feed_info.txt
    return verifyGTFS(date.year, date.month, date.day);
}
route getRouteInfo(string route_id) { // requirements: routes.txt
    route output;
    ifstream routeFile = ifstream(routePath);

    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool firstLine = true;
    std::unordered_map<string, int> refs;

    while (getline(routeFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);

        if (firstLine) {
            for (int i = 0; i < parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["route_id"]] == route_id) {
            // required fields
            output.route_id = route_id;
            output.route_type = static_cast<route::type>(to_integer(parsedCurrentLine[refs["route_type"]]));

            // optional/conditionally required/conditionally forbidden fields
            { auto find = refs.find("agency_id");
            if (find != refs.end()) output.agency_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("route_short_name");
            if (find != refs.end()) output.route_short_name = parsedCurrentLine[find->second]; }

            { auto find = refs.find("route_long_name");
            if (find != refs.end()) output.route_long_name = parsedCurrentLine[find->second]; }

            { auto find = refs.find("route_desc");
            if (find != refs.end()) output.route_desc = parsedCurrentLine[find->second]; }

            { auto find = refs.find("route_url");
            if (find != refs.end()) output.route_url = parsedCurrentLine[find->second]; }

            { auto find = refs.find("route_color");
            if (find != refs.end()) output.route_color = parsedCurrentLine[find->second]; }

            { auto find = refs.find("route_text_color");
            if (find != refs.end()) output.route_text_color = parsedCurrentLine[find->second]; }

            { auto find = refs.find("route_sort_order");
            if (find != refs.end()) output.route_sort_order = (parsedCurrentLine[find->second] == "" || parsedCurrentLine[find->second] == " ") ? 25565 : to_integer(parsedCurrentLine[find->second]); }

            { auto find = refs.find("continuous_pickup");
            if (find != refs.end()) output.continuous_pickup = static_cast<route::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }
            
            { auto find = refs.find("continuous_drop_off");
            if (find != refs.end()) output.continuous_drop_off = static_cast<route::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("network_id");
            if (find != refs.end()) output.network_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("cemv_support");
            if (find != refs.end()) output.cemv_support = static_cast<route::cemv>(to_integer(parsedCurrentLine[find->second])); }

            break;
        }
    }

    routeFile.close();
    return output;
}
stop getStopInfo(string stop_id) { // requirements: stops.txt
    ifstream stopFile = ifstream(stopPath);

    stop output;

    string currentLine;
    bool firstLine = true;
    std::vector<string> parsedCurrentLine;
    std::unordered_map<string, int> refs;

    while (getline(stopFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["stop_id"]] == stop_id) {
            // required fields
            output.stop_id = stop_id;

            // optional/conditionally required/conditionally forbidden fields
            { auto find = refs.find("stop_code");
            if (find != refs.end()) output.stop_code = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_name");
            if (find != refs.end()) output.stop_name = parsedCurrentLine[find->second]; }

            { auto find = refs.find("tts_stop_name");
            if (find != refs.end()) output.tts_stop_name = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_desc");
            if (find != refs.end()) output.stop_desc = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_lat");
            if (find != refs.end()) output.stop_lat = to_double(parsedCurrentLine[find->second]); }

            { auto find = refs.find("stop_lon");
            if (find != refs.end()) output.stop_lon = to_double(parsedCurrentLine[find->second]); }

            { auto find = refs.find("zone_id");
            if (find != refs.end()) output.zone_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_url");
            if (find != refs.end()) output.stop_url = parsedCurrentLine[find->second]; }

            { auto find = refs.find("location_type");
            if (find != refs.end()) output.location_type = static_cast<stop::location>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("parent_station");
            if (find != refs.end()) output.parent_station = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_timezone");
            if (find != refs.end()) output.stop_timezone = parsedCurrentLine[find->second]; }

            { auto find = refs.find("wheelchair_boarding");
            if (find != refs.end()) output.wheelchair_boarding = static_cast<stop::wheelchair>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("level_id");
            if (find != refs.end()) output.level_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("platform_code");
            if (find != refs.end()) output.platform_code = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_access");
            if (find != refs.end()) output.stop_access = static_cast<stop::access>(to_integer(parsedCurrentLine[find->second])); }
            break;
        }
    }


    stopFile.close();
    return output;
}
std::vector<trip_segment> getDayTimesAtStop(string stop_id, int year, int month, int day) { // requirements: stop_times.txt, routes.txt
    ifstream stopTimesFile = ifstream(stopTimesPath);
    std::vector<trip_segment> output;

    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool firstLine = true;
    std::unordered_map<string, int> refs;

    while (getline(stopTimesFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["stop_id"]] == stop_id) {
            trip_segment x;

            // required fields
            x.stop.stop_id = stop_id;
            x.stop.trip_id = parsedCurrentLine[refs["trip_id"]];
            x.stop.stop_sequence = to_integer(parsedCurrentLine[refs["stop_sequence"]]);

            // optional/conditionally required/conditionally forbidden fields
            { auto find = refs.find("arrival_time");
            if (find != refs.end()) x.stop.arrival_time = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("departure_time");
            if (find != refs.end()) x.stop.departure_time = parseFormattedTime(parsedCurrentLine[find->second]); }
            
            { auto find = refs.find("location_group_id");
            if (find != refs.end()) x.stop.location_group_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("location_id");
            if (find != refs.end()) x.stop.location_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_sequence");
            if (find != refs.end()) x.stop.stop_sequence = to_integer(parsedCurrentLine[find->second]); }

            { auto find = refs.find("stop_headsign");
            if (find != refs.end()) x.stop.stop_headsign = parsedCurrentLine[find->second]; }

            { auto find = refs.find("start_pickup_drop_off_window");
            if (find != refs.end()) x.stop.start_pickup_drop_off_window = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("end_pickup_drop_off_window");
            if (find != refs.end()) x.stop.end_pickup_drop_off_window = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("pickup_type");
            if (find != refs.end()) x.stop.pickup_type = static_cast<stop_time::pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("drop_off_type");
            if (find != refs.end()) x.stop.drop_off_type = static_cast<stop_time::pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("continuous_pickup");
            if (find != refs.end()) x.stop.continuous_pickup = static_cast<stop_time::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("continuous_drop_off");
            if (find != refs.end()) x.stop.continuous_drop_off = static_cast<stop_time::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("shape_dist_traveled");
            if (find != refs.end()) x.stop.shape_dist_traveled = to_float(parsedCurrentLine[find->second]); }

            { auto find = refs.find("timepoint");
            if (find != refs.end()) x.stop.timepoint = static_cast<stop_time::timepoint_type>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("pickup_booking_rule_id");
            if (find != refs.end()) x.stop.pickup_booking_rule_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("drop_off_booking_rule_id");
            if (find != refs.end()) x.stop.drop_off_booking_rule_id = parsedCurrentLine[find->second]; }

            output.push_back(x);
        }
    }
    stopTimesFile.close();



    ifstream tripsFile = ifstream(tripsPath);
    currentLine = "";
    parsedCurrentLine = std::vector<string>(0);
    firstLine = true;
    refs.clear();

    while (getline(tripsFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        for (trip_segment& a : output) {
            if (a.stop.trip_id == parsedCurrentLine[refs["trip_id"]]) {
                a.route_id = parsedCurrentLine[refs["route_id"]];
            }
        }
    }

    tripsFile.close();
    output.erase(std::remove_if(output.begin(), output.end(), [year, month, day](trip_segment x){ return (!isTripValid(x.stop.trip_id, year, month, day)); }), output.end());
    return output;
}
std::vector<trip_segment> getDayTimesAtStop(string stop_id, calendar_day date) { // requirements: stop_times.txt, routes.txt
    return getDayTimesAtStop(stop_id, date.year, date.month, date.day);
}
std::vector<agency> getAgencyInfo() { // requiremnts: agency.txt
    std::vector<agency> output;
    ifstream agencyFile = ifstream(agencyPath);
    string currentLine;
    std::vector<string> parsedCurrentLine;
    std::unordered_map<string, int> refs;
    int lineNo = 0;

    while (getline(agencyFile, currentLine)) {
        lineNo++;
        parsedCurrentLine = parseDataCSV(currentLine);
        if (parsedCurrentLine.empty() || (parsedCurrentLine.size() == 0 && parsedCurrentLine[0].empty())) continue;

        if (lineNo == 1) {
            refs = createMapFromVector(parsedCurrentLine);
            continue;
        } 
        agency temp;
        // required fields
        temp.agency_name = parsedCurrentLine[refs["agency_name"]];
        temp.agency_url = parsedCurrentLine[refs["agency_url"]];
        temp.agency_timezone = parsedCurrentLine[refs["agency_timezone"]];

        // optional/conditionally required/conditionally forbidden fields
        { auto find = refs.find("agency_id");
        if (find != refs.end()) temp.agency_id = parsedCurrentLine[find->second]; }

        { auto find = refs.find("agency_lang");
        if (find != refs.end()) temp.agency_lang = parsedCurrentLine[find->second]; }

        { auto find = refs.find("agency_phone");
        if (find != refs.end()) temp.agency_phone = parsedCurrentLine[find->second]; }

        { auto find = refs.find("agency_fare_url");
        if (find != refs.end()) temp.agency_fare_url = parsedCurrentLine[find->second]; }

        { auto find = refs.find("agency_email");
        if (find != refs.end()) temp.agency_email = parsedCurrentLine[find->second]; }

        { auto find = refs.find("cemv_support");
        if (find != refs.end()) temp.cemv_support = static_cast<agency::cemv>(to_integer(parsedCurrentLine[find->second])); }
        
        output.push_back(temp);        
    }

    agencyFile.close();
    return output;
}
std::vector<shape> getShapeInfo(string shape_id) { // requirements: shape.txt
    std::vector<shape> output;
    ifstream shapeFile = ifstream(shapePath);

    string currentLine;
    std::vector<string> parsedCurrentLine;
    std::unordered_map<string, int> refs;
    bool firstLine = true;

    while (getline(shapeFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);

        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }
        
        if (parsedCurrentLine[refs["shape_id"]] == shape_id) {
            shape temp;

            // requried fields
            temp.shape_id = shape_id;
            temp.shape_pt_lat = to_double(parsedCurrentLine[refs["shape_pt_lat"]]);
            temp.shape_pt_lon = to_double(parsedCurrentLine[refs["shape_pt_lon"]]);
            temp.shape_pt_sequence = to_integer(parsedCurrentLine[refs["shape_pt_sequence"]]);

            // optional field
            { auto find = refs.find("shape_dist_traveled");
            if (find != refs.end()) temp.shape_dist_traveled = to_float(parsedCurrentLine[find->second]); }

            output.push_back(temp);
        }
    }

    shapeFile.close();
    return output;
}
std::vector<trip_segment> getRemainingDayStops(string stop_id, time intime, int year, int month, int day) { // stop_times.txt, routes.txt
    std::vector<trip_segment> out = getDayTimesAtStop(stop_id, year, month, day);
    
    out.erase(std::remove_if(out.begin(), out.end(), [intime](trip_segment x){ return (x.stop.departure_time < intime); }), out.end());
    return out;
}
std::vector<trip_segment> getRemainingDayStops(string stop_id, time intime, calendar_day date) { // stop_times.txt, routes.txt
    return getRemainingDayStops(stop_id, intime, date.year, date.month, date.day);
}
std::vector<matchsearch> searchStop(const string& name) { // stops.txt
    ifstream stopFile(stopPath);

    string currentLine;
    std::map<string, int> refs;
    std::vector<intstr> stopNames;
    int lineNumber = 0;

    while (getline(stopFile, currentLine)) {
        auto parsedCurrentLine = parseDataCSV(currentLine);
        ++lineNumber;

        if (lineNumber == 1) [[unlikely]] {
            for (int i = 0; i < (int)parsedCurrentLine.size(); i++) {
                refs[parsedCurrentLine[i]] = i;
            }
        } else {
            stopNames.emplace_back(
                stoi(parsedCurrentLine[refs.at("stop_id")]),
                parsedCurrentLine[refs.at("stop_name")]
            );
        }
    }

    std::vector<matchsearch> results;
    results.reserve(stopNames.size());

    for (const auto& item : stopNames) {
        int dist   = levenshtein(item.str, name);
        int maxLen = (int)std::max(item.str.size(), name.size());
        int score  = (maxLen > 0) ? (100 - (dist * 100 / maxLen)) : 100;

        results.push_back({ item, score });
    }

    std::sort(results.begin(), results.end(),
        [](const matchsearch& a, const matchsearch& b) {
            return a.score > b.score;
        });

    return results;
}
std::vector<trip_segment> getAllStops(string trip_id) { // stop_times.txt, routes.txt
    std::vector<trip_segment> output;
    ifstream stopTimesFile = ifstream(stopTimesPath);

    string currentLine;
    bool firstLine = true;
    std::vector<string> parsedCurrentLine;
    std::unordered_map<string, int> refs;

    while (getline(stopTimesFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }
        if (parsedCurrentLine[refs["trip_id"]] == trip_id) {
            trip_segment x;

            // required fields
            x.stop.trip_id = trip_id;
            x.stop.stop_sequence = to_integer(parsedCurrentLine[refs["stop_sequence"]]);

            // optional/conditionally required/conditionally forbidden fields
            { auto find = refs.find("stop_id");
            if (find != refs.end()) x.stop.stop_id = parsedCurrentLine[refs["stop_id"]]; }

            { auto find = refs.find("arrival_time");
            if (find != refs.end()) x.stop.arrival_time = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("departure_time");
            if (find != refs.end()) x.stop.departure_time = parseFormattedTime(parsedCurrentLine[find->second]); }
            
            { auto find = refs.find("location_group_id");
            if (find != refs.end()) x.stop.location_group_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("location_id");
            if (find != refs.end()) x.stop.location_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_sequence");
            if (find != refs.end()) x.stop.stop_sequence = to_integer(parsedCurrentLine[find->second]); }

            { auto find = refs.find("stop_headsign");
            if (find != refs.end()) x.stop.stop_headsign = parsedCurrentLine[find->second]; }

            { auto find = refs.find("start_pickup_drop_off_window");
            if (find != refs.end()) x.stop.start_pickup_drop_off_window = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("end_pickup_drop_off_window");
            if (find != refs.end()) x.stop.end_pickup_drop_off_window = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("pickup_type");
            if (find != refs.end()) x.stop.pickup_type = static_cast<stop_time::pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("drop_off_type");
            if (find != refs.end()) x.stop.drop_off_type = static_cast<stop_time::pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("continuous_pickup");
            if (find != refs.end()) x.stop.continuous_pickup = static_cast<stop_time::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("continuous_drop_off");
            if (find != refs.end()) x.stop.continuous_drop_off = static_cast<stop_time::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("shape_dist_traveled");
            if (find != refs.end()) x.stop.shape_dist_traveled = to_float(parsedCurrentLine[find->second]); }

            { auto find = refs.find("timepoint");
            if (find != refs.end()) x.stop.timepoint = static_cast<stop_time::timepoint_type>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("pickup_booking_rule_id");
            if (find != refs.end()) x.stop.pickup_booking_rule_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("drop_off_booking_rule_id");
            if (find != refs.end()) x.stop.drop_off_booking_rule_id = parsedCurrentLine[find->second]; }

            output.push_back(x);
        }

    }
    stopTimesFile.close();

    ifstream tripsFile = ifstream(tripsPath);
    currentLine = "";
    parsedCurrentLine = std::vector<string>(0);
    firstLine = true;
    refs.clear();

    while (getline(tripsFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        for (trip_segment& a : output) {
            if (a.stop.trip_id == parsedCurrentLine[refs["trip_id"]]) {
                a.route_id = parsedCurrentLine[refs["route_id"]];
            }
        }
    }

    tripsFile.close();

    return output;
}
std::vector<stop> getNearestStops(double lat, double lon, int maxResults = -1, double maxDistanceKM = -1) {
    std::vector<std::pair<double, stop>> candidates;
    ifstream stopFile = ifstream(stopPath);

    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool firstLine = true;
    std::unordered_map<string, int> refs;

    while (getline(stopFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);

        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        auto latFind = refs.find("stop_lat");
        auto lonFind = refs.find("stop_lon");
        if (latFind == refs.end() || lonFind == refs.end()) continue;

        double sLat = to_double(parsedCurrentLine[latFind->second]);
        double sLon = to_double(parsedCurrentLine[lonFind->second]);
        if (sLat == 0.0 && sLon == 0.0) continue;

        double d = getDistanceKM(lat, lon, sLat, sLon);
        if (d > maxDistanceKM && maxDistanceKM != -1) continue;

        stop temp;
        temp.stop_id = parsedCurrentLine[refs["stop_id"]];
        temp.stop_lat = sLat;
        temp.stop_lon = sLon;

        { auto find = refs.find("stop_code");
        if (find != refs.end()) temp.stop_code = parsedCurrentLine[find->second]; }

        { auto find = refs.find("stop_name");
        if (find != refs.end()) temp.stop_name = parsedCurrentLine[find->second]; }

        { auto find = refs.find("tts_stop_name");
        if (find != refs.end()) temp.tts_stop_name = parsedCurrentLine[find->second]; }

        { auto find = refs.find("stop_desc");
        if (find != refs.end()) temp.stop_desc = parsedCurrentLine[find->second]; }

        { auto find = refs.find("zone_id");
        if (find != refs.end()) temp.zone_id = parsedCurrentLine[find->second]; }

        { auto find = refs.find("stop_url");
        if (find != refs.end()) temp.stop_url = parsedCurrentLine[find->second]; }

        { auto find = refs.find("location_type");
        if (find != refs.end()) temp.location_type = static_cast<stop::location>(to_integer(parsedCurrentLine[find->second])); }

        { auto find = refs.find("parent_station");
        if (find != refs.end()) temp.parent_station = parsedCurrentLine[find->second]; }

        { auto find = refs.find("stop_timezone");
        if (find != refs.end()) temp.stop_timezone = parsedCurrentLine[find->second]; }

        { auto find = refs.find("wheelchair_boarding");
        if (find != refs.end()) temp.wheelchair_boarding = static_cast<stop::wheelchair>(to_integer(parsedCurrentLine[find->second])); }

        { auto find = refs.find("level_id");
        if (find != refs.end()) temp.level_id = parsedCurrentLine[find->second]; }

        { auto find = refs.find("platform_code");
        if (find != refs.end()) temp.platform_code = parsedCurrentLine[find->second]; }

        { auto find = refs.find("stop_access");
        if (find != refs.end()) temp.stop_access = static_cast<stop::access>(to_integer(parsedCurrentLine[find->second])); }

        candidates.emplace_back(d, std::move(temp));
    }

    stopFile.close();

    std::sort(candidates.begin(), candidates.end(),
        [](const std::pair<double, stop>& a, const std::pair<double, stop>& b) {
            return a.first < b.first;
        });

    std::vector<stop> output;
    int limit = maxResults == -1 ? (int)candidates.size() : std::min((int)candidates.size(), maxResults);
    output.reserve(limit);
    for (int i = 0; i < limit; i++) {
        output.push_back(std::move(candidates[i].second));
    }

    return output;
}
std::vector<trip> getAllTrips(string route_id) {
    std::vector<trip> output;

    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool firstLine = true;
    std::unordered_map<string, int> refs;


    ifstream tripFile = ifstream(tripPath);

    while (getline(tripFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);

        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["route_id"]] == route_id) {
            trip temp;
            temp.trip_id = parsedCurrentLine[refs["trip_id"]];

            output.push_back(temp);
        }
    }

    tripFile.close();
    return output;
}
service getServiceInfo(string service_id) {
    service output;
    output.schedule.service_id = service_id;

    ifstream calendarFile = ifstream(calendarPath);
    ifstream calendarDatesFile = ifstream(calendarDatesPath);

    bool firstLine = true;
    string currentLine;

    std::vector<string> parsedCurrentLine;

    std::unordered_map<string, int> refs;
    while (getline(calendarFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);

        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["service_id"]] == service_id) {
            output.schedule.start_date = parseFormattedDate(parsedCurrentLine[refs["start_date"]]);
            output.schedule.end_date = parseFormattedDate(parsedCurrentLine[refs["end_date"]]);

            output.schedule.monday = static_cast<bool>(stoi(parsedCurrentLine[refs["monday"]]));
            output.schedule.tuesday = static_cast<bool>(stoi(parsedCurrentLine[refs["tuesday"]]));
            output.schedule.wednesday = static_cast<bool>(stoi(parsedCurrentLine[refs["wednesday"]]));
            output.schedule.thursday = static_cast<bool>(stoi(parsedCurrentLine[refs["thursday"]]));
            output.schedule.friday = static_cast<bool>(stoi(parsedCurrentLine[refs["friday"]]));
            output.schedule.saturday = static_cast<bool>(stoi(parsedCurrentLine[refs["saturday"]]));
            output.schedule.sunday = static_cast<bool>(stoi(parsedCurrentLine[refs["sunday"]]));
            break; // accoring to google the service id SHOULD show up only once in calendar.txt
        }
    }

    calendarFile.close();

    currentLine = "";
    parsedCurrentLine = std::vector<string>(0);
    firstLine = true;

    refs = std::unordered_map<string, int>();

    while (getline(calendarDatesFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);

        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            
            continue;
        }

        if (parsedCurrentLine[refs["service_id"]] == service_id) {
            calendar_date::exception x = static_cast<calendar_date::exception>(std::stoi(parsedCurrentLine[refs["exception_type"]]));
            calendar_day y = parseFormattedDate(parsedCurrentLine[refs["date"]]);
            
            calendar_date temp;
            temp.exception_type = x;
            temp.date = y;
            temp.service_id = service_id;
            output.exceptions.push_back(temp);
            continue;
        }
    }
    calendarDatesFile.close();
    return output;
}
std::vector<trip> getAllBlockId(string block_id) { // requirements: trips.txt with optional field block_id
    std::vector<trip> output;
    ifstream tripFile = ifstream(tripPath);

    string currentLine;
    std::vector<string> parsedCurrentLine;
    std::unordered_map<string, int> refs;
    bool firstLine = true;

    while (getline(tripFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);
        
        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }

        if (parsedCurrentLine[refs["block_id"]] == block_id) {
            trip temp;
            temp.trip_id = parsedCurrentLine[refs["trip_id"]];

            output.push_back(temp);
        }
    }
    tripFile.close();
    return output;
}
std::vector<trip_segment> getStopTimeInfo(string trip_id) { // requiremtns: stop_time.txt
    std::vector<trip_segment> output; // assume a trip only appears once in
    ifstream stopTimesFile = ifstream(stopTimesPath);

    string currentLine;
    std::vector<string> parsedCurrentLine;
    bool firstLine = true;
    std::unordered_map<string, int> refs;

    while (getline(stopTimesFile, currentLine)) {
        parsedCurrentLine = parseDataCSV(currentLine);

        if (firstLine) {
            refs = createMapFromVector(parsedCurrentLine);
            firstLine = false;
            continue;
        }
        

        if (parsedCurrentLine[refs["trip_id"]] == trip_id) {
            trip_segment x;

            // required fields
            x.stop.trip_id = trip_id;
            x.stop.stop_sequence = to_integer(parsedCurrentLine[refs["stop_sequence"]]);

            // optional/conditionally required/conditionally forbidden fields
            { auto find = refs.find("stop_id");
            if (find != refs.end()) x.stop.stop_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("arrival_time");
            if (find != refs.end()) x.stop.arrival_time = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("departure_time");
            if (find != refs.end()) x.stop.departure_time = parseFormattedTime(parsedCurrentLine[find->second]); }
            
            { auto find = refs.find("location_group_id");
            if (find != refs.end()) x.stop.location_group_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("location_id");
            if (find != refs.end()) x.stop.location_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("stop_sequence");
            if (find != refs.end()) x.stop.stop_sequence = to_integer(parsedCurrentLine[find->second]); }

            { auto find = refs.find("stop_headsign");
            if (find != refs.end()) x.stop.stop_headsign = parsedCurrentLine[find->second]; }

            { auto find = refs.find("start_pickup_drop_off_window");
            if (find != refs.end()) x.stop.start_pickup_drop_off_window = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("end_pickup_drop_off_window");
            if (find != refs.end()) x.stop.end_pickup_drop_off_window = parseFormattedTime(parsedCurrentLine[find->second]); }

            { auto find = refs.find("pickup_type");
            if (find != refs.end()) x.stop.pickup_type = static_cast<stop_time::pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("drop_off_type");
            if (find != refs.end()) x.stop.drop_off_type = static_cast<stop_time::pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("continuous_pickup");
            if (find != refs.end()) x.stop.continuous_pickup = static_cast<stop_time::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("continuous_drop_off");
            if (find != refs.end()) x.stop.continuous_drop_off = static_cast<stop_time::continuous_pickup_dropoff>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("shape_dist_traveled");
            if (find != refs.end()) x.stop.shape_dist_traveled = to_float(parsedCurrentLine[find->second]); }

            { auto find = refs.find("timepoint");
            if (find != refs.end()) x.stop.timepoint = static_cast<stop_time::timepoint_type>(to_integer(parsedCurrentLine[find->second])); }

            { auto find = refs.find("pickup_booking_rule_id");
            if (find != refs.end()) x.stop.pickup_booking_rule_id = parsedCurrentLine[find->second]; }

            { auto find = refs.find("drop_off_booking_rule_id");
            if (find != refs.end()) x.stop.drop_off_booking_rule_id = parsedCurrentLine[find->second]; }

            output.push_back(x);
        }
    }


    stopTimesFile.close();
    return output;
}

#pragma endregion FUNCTIONS

}; // MARK: END OF NAMESPACE GTFS

#endif