#include <iostream>
#include <string>
#include <map>

using namespace std;

std::map<string, int> createMapFromVector(std::vector<string>& param) {
    int length = param.capacity();
    std::map<string, int> output;
    for (int i = 0; i < length; i++) {
        if (param[i] == "" || param[i] == " ") {
            continue;
        } else { output[param[i]] = i; }
    }    
    return output;
}

int main(int argc, char* argv[]) {
    map<string, int> ohio;
    vector<string> yes = {"hello", "rizz", "sigma", "ohio", "skibidi", "6", "7", "9", "bus", "skibid", "oscar", "door", "wheelchair_friendly", "wheelchair_boarding", "preferred", "", " ", "", "  "};

    ohio = createMapFromVector(yes);

    for (const auto& a : ohio) {
        cout << a.first << ": " << a.second << endl;
    }
    return 0;
}