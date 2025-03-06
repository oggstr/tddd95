/**
 * @author Oskar Arensmeier
 * @date 2024-02-27
 */

#include <iostream>
#include <vector>
#include <climits>
#include <bitset>
#include <map>

using namespace std;

struct edge {
    int dest;
    int weight;
};

struct query {
    int start;
    int end;
    int fuel;
};

int N, M, Q;

vector<int> cities;
map<int, vector<edge>> graph;
vector<query> queries;

int dfs(
    int v,
    int fuel,
    int cost,
    int end,
    int cap,
    bitset<1000> & visited
) {
    if (v == end)
        return cost;

    visited[v] = true;
    int best = INT_MAX;
    for (auto & e : graph[v]) {
        if (visited[e.dest])
            continue;

        for (int f = fuel; f <= cap; ++f) {
            int new_cost = cost + cities[v] * (f - fuel);
            int new_fuel = f - e.weight;

            if (new_fuel < 0)
                continue;

            best = min(
                best,
                dfs(
                    e.dest,
                    new_fuel,
                    new_cost,
                    end,
                    cap,
                    visited
                )
            );
        }
    }
    visited[v] = false;

    return best;
}

int main()
{
    cin >> N >> M;

    cities.reserve(N);
    for (int i = 0; i < N; ++i) {
        int c;
        cin >> c;
        cities.push_back(c);
    }

    for (int i = 0; i < M; ++i) {
        int v, u, w;
        cin >> v >> u >> w;

        graph[v].push_back(edge{u, w});
        graph[u].push_back(edge{v, w});
    }

    cin >> Q;
    queries.reserve(Q);
    for (int i = 0; i < Q; ++i) {
        int cap, start, end;
        cin >> cap >> start >> end;

        bitset<1000> visited;
        int res = dfs(start, 0, 0, end, cap, visited);

        if (res == INT_MAX) {
            cout << "impossible\n";
        } else {
            cout << res << '\n';
        }
    }

    cout << flush;
    return 0;
}