#include <iostream>
#include <string>

using namespace std;

double getscore(string input) {
    std::vector<int> weights = {1};
    double total = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] >= 'A' && input[i] <= 'Z') {
            input[i] = input[i] + 32;
        }
    }
    for (int i = 0; i < input.length(); i++) {
        total += static_cast<double>(input[i]) * static_cast<double>(weights[i % weights.size()]) - 48.0;
    }

    total /= input.length();
    return total;

}

int main() {
    cout << getscore("yonge st / bond cres") - getscore("bond cres / yonge st");
    return 0;
}