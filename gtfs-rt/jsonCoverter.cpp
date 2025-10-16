#include <iostream>
#include <string>
#include <fstream>

using namespace std;


class base {
    string name;
    int level;
    base parent;
    base(string name) {
        this->name = name;
        level = 0;
    }
    virtual ~base() {}
};

class branch : public base {
    base parent;

};
int main(int argc, char* argv[]) {

}