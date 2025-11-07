#include <iostream>
#include <vector>
#include <algorithm> // for remove_if
using namespace std;

int main() {
    vector<int> f = {0, 1, 2, 3, 4, 5, 6, 1, 2, 4, 6, 1, 3, 6, 100};

    for (int i : f) cout << i << " ";
    cout << endl;

    f.erase(remove_if(f.begin(), f.end(), [](int x){ return x > 3; }), f.end());

    for (int i : f) cout << i << " ";
    cout << endl;
}