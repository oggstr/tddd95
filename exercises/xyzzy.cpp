/**
 * @author Oskar Arensmeier
 * @date 2024-03-06
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

#define ll int
#define oo INT_MAX
#define SZ 101

using namespace std;

int N;

vector<int> graph[SZ];
ll energy[SZ];
ll dp[SZ];

bool bfs()
{
    dp[1] = 100;

    queue<pair<int, int>> q;
    q.push({1, 100});

    while (! q.empty()) {
        auto [u, e] = q.front();
        q.pop();

        if (u == N) {
            return true;
        }

        for (int v : graph[u]) {
            if (e + energy[v] > 0 && dp[v] < e + energy[v]) {
                dp[v] = e + energy[v];
                q.push({v, e + energy[v]});
            }
        }
    }

    return false;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    while (true) {
        cin >> N; 
        if (N == -1)
            break;

        fill(&dp[0], &dp[0] + SZ, -oo);
        for (int i = 0; i < SZ; ++i) {
            graph[i].clear();
        }

        for (int i = 1; i <= N; ++i) {
            int e, m;
            cin >> e >> m;
            energy[i] = e;

            for (int j = 0; j < m; ++j) {
                int x;
                cin >> x;
                graph[i].push_back(x);
            }
        }

        if (bfs()) {
            cout << "winnable\n";
        } else {
            cout << "hopeless\n";
        }
    }

    cout << flush;
    return 0;
}