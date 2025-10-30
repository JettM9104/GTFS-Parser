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

    virtual bool operator<=(time24 other) {
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
    
    virtual bool operator>=(time24 other) {
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

    virtual bool operator==(time24 other) {
        return (other.h == this->h && other.m == this->m && other.s == this->s);
    }

    virtual bool operator!=(time24 other) {
        return !(other.h == this->h && other.m == this->m && other.s == this->s);
    }

    virtual time24 operator+(time24 other) { // cannot overlap, note that during calling
        time24 output;

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

    virtual time24 operator+(double other) {
        time24 output(this->h, this->m, this->s);

        
    }
};




int main() {
    time24 a(10, 3, 4);
    time24 b(10, 3, 5);

    cout << (b > a);
    return 0;
}