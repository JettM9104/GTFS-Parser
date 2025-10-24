#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    vector<int> gay = {0, 1, 2, 3};

    for (int i = 0; i < gay.size(); i++) {
        cout << gay[i] << " ";
    }
    cout << endl;

    gay.erase(gay.begin() + 1);
    
    for (int i = 0; i < gay.size(); i++) {
        cout << gay[i] << " ";
    }
    cout << endl;
    return 0;
}