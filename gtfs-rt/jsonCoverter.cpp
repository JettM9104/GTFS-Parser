#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef enum {ADDED = 0, SCHEDULED, UNSCHEDULED, CANCELLED, REPLACEMENT, DUPLICATED, NEW, DELETED} schedule_relationship;

class base {
    string name;
    int level;
    base(string name) {
        this->name = name;
        level = 0;
    }
    virtual ~base() {}
};

class  {
    ;
};
int main(int argc, char* argv[]) {

}