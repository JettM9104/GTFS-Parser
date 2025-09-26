#include <iostream>
using namespace std;

string convertDatesToWords(const bool &mon, const bool &tue, const bool &wed, const bool &thu, const bool &fri, const bool &sat, const bool &sun);


int main(int argc, char* argv[]) {
    cout << convertDatesToWords(0, 1, 0, 0, 0, 0, 0);
}

string convertDatesToWords(const bool &mon, const bool &tue, const bool &wed, const bool &thu, const bool &fri, const bool &sat, const bool &sun) {
    bool list[7] = {mon, tue, wed, thu, fri, sat, sun};
    string days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    if (mon && tue && wed && thu && fri && sat && sun) {
        return "Every Day";
    } else if (mon && tue && wed && thu && fri && !sat && !sun) {
        return "Weekdays Only";
    } else if (mon && tue && wed && thu && fri && sat && !sun) {
        return "Monday to Saturday";
    } else if ((mon + tue + wed + thu + fri + sat + sun) == 1) {
        for (int i = 0; i < 7; i++) {
            if (list[i]) {
                return days[i] + " Only";
            }
        } 
    }
    return "Never";
}


