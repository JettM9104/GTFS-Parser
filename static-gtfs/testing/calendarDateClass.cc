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
};

int main() {
    calendarDate x(2025, 12, 03);
    calendarDate y(2025, 12, 02);

    cout << (x<=y ? "yes" : "no") ;
    
}