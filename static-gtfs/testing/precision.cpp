#include <iostream>
#include <iomanip>

using namespace std;


int main() {
    string x = "6.3242304823094832904892034890";

    double a = stod(x);

    cout << setprecision(17) << a;
    return 0;
}