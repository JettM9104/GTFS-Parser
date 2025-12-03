#include "gtfs.hpp"

int main(int argc, char* argv[]) {

    std::vector<matchsearch> x = searchStopFromScoreAlg2("BNOD CRES");

    for (int i = x.size() - 1; i >= 0; i--) {
        cout << i << ": " << x[i].score << " | " << x[i].text.str << " | ";
        cout << x[i].text.num << std::endl;
    }
}