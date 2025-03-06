/**
 * @author Oskar Arensmeier
 * @date 2024-02-27
 */

#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <bitset>
#include <map>

using namespace std;

struct edge {
    int dest;
    int weight;
};

struct car {
    int node;
    int fuel;
    int cost;
};

int N, M, Q;

vector<int> cities;
map<int, vector<edge>> graph;

int dijkstra(
    int start,
    int end,
    int cap
) {
    auto comp_car = [](car const & a, car const & b) {
        return a.cost > b.cost;
    };

    vector<vector<int>> dj(N, vector<int>(cap + 1, INT_MAX));
    dj[start][0] = 0;

    priority_queue<car, vector<car>, decltype(comp_car)> pq(comp_car);
    pq.push(car{start, 0, 0});

    while (! pq.empty()) {
        car c = pq.top();
        pq.pop();

        if (c.cost > dj[c.node][c.fuel]) {
            continue;
        }

        for (auto & e : graph[c.node]) {

            if (c.fuel >= e.weight) {
                car new_car = {e.dest, c.fuel-e.weight, c.cost};

                if (new_car.cost < dj[new_car.node][new_car.fuel]) {
                    dj[new_car.node][new_car.fuel] = new_car.cost;
                    pq.push(new_car);
                }
            }
        }

        if (c.fuel < cap) {
            car new_car = {c.node, c.fuel+1, c.cost+cities[c.node]};
            pq.push(new_car);
        }
    }

    return dj[end][0];
}

int main()
{
    //freopen("../input/full_tank2.txt", "r", stdin);

    cin >> N >> M;

    cities.reserve(N);
    for (int i = 0; i < N; ++i) {
        int c;
        cin >> c;
        cities.push_back(c);
    }

    /* for (int i = 0; i < N; ++i) {
        graph[i] = vector<edge>();
    } */

    for (int i = 0; i < M; ++i) {
        int v, u, w;
        cin >> v >> u >> w;

        graph[v].push_back(edge{u, w});
        graph[u].push_back(edge{v, w});
    }

    cin >> Q;
    for (int i = 0; i < Q; ++i) {
        int cap, start, end;
        cin >> cap >> start >> end;

        int res = dijkstra(start, end, cap);

        if (res == INT_MAX) {
            cout << "impossible\n";
        } else {
            cout << res << '\n';
        }
    }

    cout << flush;
    return 0;
}