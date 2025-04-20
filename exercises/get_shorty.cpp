/**
 * @author Oskar Arensmeier
 * @date 2024-03-06
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <set>

#define SZ 10000

using namespace std;

int N, M;

vector<pair<int, double>> graph[SZ];

vector<double> dijkstra() {
    vector<double> dist(N, INT_MAX);
    vector<pair<int, double>> backtrace(N, {-1, 0.0}); // Store path and original weight

    dist[0] = 0;

    set<pair<double, int>> q;
    q.insert({0.0, 0});

    while (! q.empty()) {
        int u = q.begin()->second;
        q.erase(q.begin());

        for (auto edge : graph[u]) {
            int v    = edge.first;
            double w = edge.second;

            if (dist[u] + w < dist[v]) {
                q.erase({dist[v], v});
                dist[v] = dist[u] + w;
                backtrace[v] = {u, w};
                q.insert({dist[v], v});
            }
        }
    }

    vector<double> weights;
    int v = N - 1;
    while (v != -1) {
        weights.push_back(backtrace[v].second);
        v = backtrace[v].first;
    }

    return weights;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    while (cin >> N >> M) {
        if (N == 0 && M == 0) {
            break;
        }

        for (int i = 0; i < N; ++i) {
            graph[i].clear();
        }

        for (int i = 0; i < M; ++i) {
            int x, y;
            double f;
            cin >> x >> y >> f;

            // Log values so that we can use dijkstra
            // (negative since we want to maximize the value)
            graph[x].push_back({y, -log(f)});
            graph[y].push_back({x, -log(f)});
        }

        vector<double> weights = dijkstra();

        double res = 1.0;
        for (int i = weights.size() - 1; i >= 0; --i) {
            // convert back to original value
            res *= exp(-weights[i]);
        }

        cout << fixed;
        cout.precision(4);
        cout << res << '\n';
    }

    cout << flush;
    return 0;
}