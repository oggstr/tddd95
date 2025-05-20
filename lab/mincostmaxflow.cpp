#include <iostream>
#include <vector>
#include <limits>
#include <queue>
// #include "../util/bellmanford.cpp"

using namespace std;

using ll = long long;

const ll INF = numeric_limits<ll>::max() / 2;

class Edge {
public:
    /**
     * From vertex
     */
    int v;

    /**
     * To vertex
     */
    int u;

    /**
     * Cost
     */
    int cost;

    /**
     * Capacity
     */
    int cap;
};

/**
 * List of edges
 */
using EdgeList = vector<Edge>;

/**
 * Successive shortest path algorithm
 */
class SSP {

public:

    /**
     * Edges
     */
    EdgeList edges;

    /**
     * Node count
     */
    int n;

    SSP(int n, EdgeList edges) : n(n), edges(edges)
    {}

    /**
     * Minimum cost maximum flow
     *
     * @param src Source
     * @param sink Sink
     * @param K Limit flow - default INF, for max flow
     */
    pair<int, int> min_cost_max_flow(
        int src,
        int sink,
        int K = numeric_limits<int>::max()
    ) {
        vector<vector<int>> adj(n, vector<int>());
        vector<vector<int>> cost(n, vector<int>(n, 0));
        vector<vector<int>> cap(n, vector<int>(n, 0));

        for (auto & e : edges) {
            adj[e.u].push_back(e.v);
            adj[e.v].push_back(e.u);

            cost[e.u][e.v] = e.cost;
            cost[e.v][e.u] = -e.cost;

            cap[e.u][e.v] = e.cap;
        }

        // Total flow and cost
        int total_flow = 0;
        int total_cost = 0;

        vector<ll> dist;
        vector<int> parent;

        while (total_flow < K) {
            shortest_paths(src, adj, cost, cap, dist, parent);

            // Check if no augmenting paths
            if (dist[sink] == INF) {
                break;
            }

            // Find max flow on augmenting path
            int f = K - total_flow;
            int curr = sink;
            while (curr != src) {
                f = min(f, cap[parent[curr]][curr]);
                curr = parent[curr];
            }

            total_flow += f;
            total_cost += f * dist[sink];

            // Adjust capacities according to
            // altered flow
            curr = sink;
            while (curr != src) {
                cap[parent[curr]][curr] -= f;
                cap[curr][parent[curr]] += f;
                curr = parent[curr];
            }
        }

        return {total_flow, total_cost};
    }

private:

    void dijkstra(
        int src,
        vector<vector<int>> & adj,
        vector<vector<int>> & cost,
        vector<vector<int>> & cap,
        vector<ll> & dist,
        vector<int> & parent,
        vector<int> & potentials
    ) {}

    void shortest_paths(
        int src,
        vector<vector<int>> & adj,
        vector<vector<int>> & cost,
        vector<vector<int>> & cap,
        vector<ll> & dist,
        vector<int> & parent
    ) {
        parent.assign(n, -1);

        dist.assign(n, INF);
        dist[src] = 0;

        vector<bool> in_queue(n, false);
        queue<int> q;

        q.push(src);
        while (! q.empty()) {
            int u = q.front();
            q.pop();

            in_queue[u] = false;

            for (int v : adj[u]) {

                // Skip if no capacity or this path is worse
                if (cap[u][v] <= 0 ||
                    dist[v] <= dist[u] + (ll) cost[u][v]) {
                    continue;
                }
                dist[v] = dist[u] + cost[u][v];
                parent[v] = u;

                if (! in_queue[v]) {
                    q.push(v);
                    in_queue[v] = true;
                }
            }
        }
    }
};

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    EdgeList edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].cap >> edges[i].cost;
    }

    SSP ssp(n, edges);
    auto [flow, cost] = ssp.min_cost_max_flow(s, t);

    cout << flow << " " << cost << "\n";
    cout << flush;

    return 0;
}