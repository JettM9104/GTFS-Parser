#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

int main() {
    ifstream infile("TripUpdates.pb", ios::binary);
    ofstream outfile("liveUpdates.txt");

    outfile << "TripID,Time,Date,StopSequence,StopID\n";

    string content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());

    regex number_regex(R"(\d{4,})");
    sregex_iterator it(content.begin(), content.end(), number_regex);
    sregex_iterator end;

    string tripID = "";
    string date = "20250927";
    string time = "21:12:00";
    int stopSeq = 1;

    for (; it != end; ++it) {
        string num = it->str();
        if (tripID.empty()) {
            tripID = num;
        } else {
            outfile << tripID << "," << time << "," << date << "," << stopSeq << "," << num << "\n";
            stopSeq++;
        }
    }

    infile.close();
    outfile.close();

    cout << "Conversion done!\n";
    return 0;
}
