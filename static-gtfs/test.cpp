#include <iostream>
#include <map>

using namespace std;

int main() {
    std::unordered_map<string, int> poopoo;

    poopoo["hello"] = 3;

    cout << poopoo["QW"];
}