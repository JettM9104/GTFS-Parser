#include <iostream>
using namespace std;

int main() {
    int x = 9;

    void* vx = &x;

    cout << *(static_cast<int*>(vx));
}