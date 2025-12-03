#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// -----------------------------
// Levenshtein Distance Function
// -----------------------------
int levenshtein(const std::string &a, const std::string &b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1));

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            dp[i][j] = std::min({ 
                dp[i-1][j] + 1,     // delete
                dp[i][j-1] + 1,     // insert
                dp[i-1][j-1] + cost // replace
            });
        }
    }

    return dp[m][n];
}

// -----------------------------
// Fuzzy Search Result Structure
// -----------------------------
struct Match {
    std::string text;
    int score;
};

int main() {
    std::vector<std::string> data = {
        "YONGE ST / BOND CRES", "YONGE STREET / BNOD CRES", "BNOD CRES / YONGE ST", "application", "apply", "grape"
    };

    std::string query = "BOND"; // typo of "apple"

    std::vector<Match> results;

    for (auto &item : data) {
        int dist = levenshtein(item, query);
        int maxLen = std::max(item.size(), query.size());

        // Convert distance to a similarity score (0–100)
        int score = 100 - (dist * 100 / maxLen);

        results.push_back({item, score});
    }

    // Sort high score → low score
    std::sort(results.begin(), results.end(),
              [](auto &a, auto &b) { return a.score > b.score; });

    // Print results
    for (auto &r : results) {
        std::cout << r.text << "  (score " << r.score << ")\n";
    }
}
