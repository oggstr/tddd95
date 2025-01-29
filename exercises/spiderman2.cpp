/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-22
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

const int INF = INT_MAX;

int costs [40] = {0};

int N;
int M;

string solve()
{
    int    dp[41][1001];
    char path[41][1001];
    for (int i = 0; i < 41; ++i) {
        for (int j = 0; j < 1001; ++j) {
            dp[i][j] = INF;
            path[i][j] = ' ';
        }
    }

    dp[0][0] = 0;

    for (int i = 0; i < M; ++i) {
        for (int h = 0; h < 1001; ++h) {
            if (dp[i][h] == INF)
                continue;

            // Update up if this iteration has better solution,
            // (keeping in mind that up may change the current solution)
            // compared to dp[i][up]
            int up = h + costs[i];
            if (max(up, dp[i][h]) < dp[i+1][up]) {
                dp  [i+1][up] = max(up, dp[i][h]);
                path[i+1][up] = 'U';
            }

            // Update down if this iteration has a better solution,
            // compared to dp[i+1][down]
            int down = h - costs[i];
            if (down >= 0 && dp[i][h] < dp[i+1][down]) {
                dp  [i+1][down] = dp[i][h];
                path[i+1][down] = 'D';
            }
        }
    }

    if (dp[M][0] == INF) {
        return string("IMPOSSIBLE");
    }

    // Compute solution from end
    string sol;
    int h = 0;
    for (int i = M; i > 0; --i) {
        sol += path[i][h];

        // U -> go down to find previous
        if (path[i][h] == 'U') {
            h -= costs[i-1];
        // D -> go up to find previous
        } else {
            h += costs[i-1];
        }
    }

    reverse(sol.begin(), sol.end());
    return sol;
}

int main()
{
    scanf("%d", &N);

    for (int i = 0; i < N; ++i) {
        scanf("%d", &M);

        for (int j = 0; j < M; ++j) {
            scanf("%d", &costs[j]);
        }

        printf("%s\n", solve().c_str());
    }

    return 0;
}