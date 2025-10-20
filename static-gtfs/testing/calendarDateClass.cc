#include <iostream>
using namespace std;

class calendarDate {
public:
    int year;
    int month;
    int day;

    calendarDate() = default;
    calendarDate(int year, int month, int day) {
        this->year = year;
        this->month = month;
        this->day = day;
    }

    void printInfo() {
        cout << "year: " << year << std::endl;
        cout << "month: " << month << std::endl;
        cout << "day: " << day << std::endl;
    }
    bool operator< (const calendarDate& other) {
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
    bool operator> (const calendarDate& other) {
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


    bool operator<= (const calendarDate& other) {
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

    bool operator>= (const calendarDate& other) {
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

    bool operator== (const calendarDate& other) {
        return (other.year == this->year && other.month == this->month && other.day == this->day);
    }

    calendarDate operator+ (const calendarDate& other) {
        calendarDate output;

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

int main() {
    calendarDate x(2025, 12, 3);
    calendarDate y(0, 1, 1);

    calendarDate z = x + y;

    z.printInfo();
    
}