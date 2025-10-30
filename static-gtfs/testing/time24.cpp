#include <iostream>
using namespace std;
typedef enum {am, pm} tod;

class time24 {
public:
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
    time24() = default;
    time24(int h, int m, int s) {
        this->h = h;
        this->m = m;
        this->s = s;
    }

    virtual bool operator>(time24 other) {
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
    
    virtual bool operator<(time24 other) {

    }

    virtual bool operator<=(time24 other) {

    }
    
    virtual bool operator>=(time24 other) {

    }

    virtual bool operator==(time24 other) {

    }

    virtual time24 operator+(time24 other) { // cannot overlap, note that during calling

    }
};




int main() {
    time24 a(10, 3, 4);
    time24 b(10, 3, 5);

    cout << (b > a);
    return 0;
}