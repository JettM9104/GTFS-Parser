#include <iostream>
#include <cmath>

using namespace std;

struct time24 {
    int h;
    int m;
    float s;

    void printRawValues() const {
        cout << "h: " << h << "\nm: " << m << "\ns: " << s << endl;
    }

    string formattedTime() const {
        return to_string(h) + ":" + to_string(m) + ":" + to_string((int)s);
    }
};

struct calendarDate {
    int year;
    int month;
    int day;

    calendarDate() = default;
    calendarDate(int y, int m, int d) : year(y), month(m), day(d) {}
};

struct dateTime {
    calendarDate date;
    time24 time;
};

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int year, int month) {
    int dim[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 2 && isLeapYear(year)) return 29;
    return dim[month];
}

dateTime calculatePOSIX(unsigned long POSIX, short timeZone = 0) {
    dateTime output;

    POSIX += timeZone * 3600;

    output.time.s = POSIX % 60;
    output.time.m = (POSIX / 60) % 60;
    output.time.h = (POSIX / 3600) % 24;

    unsigned long days = POSIX / 86400;

    int year = 1970;
    while (true) {
        int daysInYear = isLeapYear(year) ? 366 : 365;
        if (days >= daysInYear) {
            days -= daysInYear;
            year++;
        } else {
            break;
        }
    }
    output.date.year = year;

    int month = 1;
    while (true) {
        int dim = daysInMonth(year, month);
        if (days >= dim) {
            days -= dim;
            month++;
        } else {
            break;
        }
    }
    output.date.month = month;
    output.date.day = days + 1;

    return output;
}

int main() {
    dateTime skibvidi = calculatePOSIX(9472389234, 0); // UTC
    cout << "Date: " << skibvidi.date.year << "-" << skibvidi.date.month << "-" << skibvidi.date.day << endl;
    cout << "Time: " << skibvidi.time.formattedTime() << endl;
}
