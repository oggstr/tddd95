#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include "../util/bellmanford.cpp"

/**
 * @author Oskar Arensmeier
 * @date 2025-05-21
 */

/**
 * Implementation of successive shortest path for
 * multi graphs using potentials (through Bellman-Ford) to
 * allow for dijkstra to be used. Solves min cost max flow.
 *
 * Algorithm:
 * The algorithm is in large parts based on: https://cp-algorithms.com/graph/min_cost_flow.html
 *
 * We start by creating a residual graph where back edges are
 * added for each edge (with negative cost and 0 capcity).
 * We then go on to run Bellman-Ford to find the shortest path
 * from the source to all other nodes. This is used to
 * initialize the potentials for the dijkstra algorithm.
 *
 * The core of the algorithms runs dijkstra to find an augmenting
 * path. Once there are no more augmenting paths, we stop.
 *
 * Time complexity:
 * - O(m * n) Pre-processing (running Bellman-Ford)
 * - O(m * log(n)) For the core algorithm
 * 
 * Total: O(m * n + m * log(n))
 *
 * Where n is nodes and m is edges.
 *
 * Space complexity:
 * - O(n * m) for the adjacency list
 * - O(n) for the distance, parent and potential arrays
 * 
 * Total: O(n * m + n) = O(n * m)
 * 
 * Data structures:
 * - Adjacency list using vectors
 * - Vectors for the distance, parent and potential arrays
 */

using namespace std;

using ll = long long;

const ll INF = numeric_limits<ll>::max() / 2;

class Edge {
public:
    /**
     * From vertex
     */
    int u;

    /**
     * To vertex
     */
    int v;

    /**
     * Cost
     */
    int cost;

    /**
     * Capacity
     */
    int cap;

    Edge(int u, int v, int cost, int cap) : u(u), v(v), cost(cost), cap(cap)
    {}
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
    class ResidualEdge {
    public:
        /**
         * Target node
         */
        int to;

        /**
         * Index in adjacency list
         * of the reverse edge
         */
        int rev_idx;

        /**
         * Cost
         */
        int cost;

        /**
         * Capacity
         */
        int cap;

        ResidualEdge(int to, int rev_idx, int cost, int cap) :
            to(to), rev_idx(rev_idx), cost(cost), cap(cap)
        {}
    };

    /**
     * Result of running the algorithm
     */
    class Result {
    public:
        /**
         * Flow
         */
        int flow;

        /**
         * Cost
         */
        int cost;

        /**
         * Flow matrix
         */
        vector<vector<int>> flow_graph;
    };

    /**
     * Edges
     */
    EdgeList edges;

    /**
     * Node count
     */
    int n;

    /**
     * Constructor
     *
     * @param n Number of nodes
     * @param edges List of edges
     */
    SSP(int n, EdgeList edges) : n(n), edges(edges)
    {}

    /**
     * Minimum cost maximum flow
     *
     * @param src Source
     * @param sink Sink
     * @param K Limit flow - default INF, for max flow
     * @return {flow, cost, flow matrix}
     */
    Result min_cost_max_flow(
        int src,
        int sink,
        int K = numeric_limits<int>::max()
    ) {
        // Multi graph adjacency list of original
        // and residual edges
        vector<vector<ResidualEdge>> adj(n);

        for (auto & e : edges) {
            int u_idx = adj[e.u].size();
            int v_idx = adj[e.v].size();

            adj[e.u].emplace_back(e.v, v_idx, e.cost, e.cap);
            adj[e.v].emplace_back(e.u, u_idx, -e.cost, 0);
        }

        /* 
        parent[v] = {u, idx},
        Where adj[v][idx] = parent edge of v.
        
        This is important since the graph is a multi graph,
        and we need to know exactly which edge was used by dijkstra.
        */
        vector<pair<int, int>> parent;

        vector<ll> dist;
        vector<vector<int>> flow(n, vector<int>(n, 0)); // Flow matrix
        vector<ll> potentials = compute_potentials(src, adj);

        int total_flow = 0;
        int total_cost = 0;
        while (total_flow < K) {
            dijkstra(src, adj, dist, parent, potentials);

            // Check if no augmenting paths
            if (dist[sink] == INF) {
                break;
            }

            // Update potentials by offsetting
            // by new distances - this ensures
            // dijkstra can be run again
            for (int i = 0; i < n; ++i) {
                if (dist[i] == INF) {
                    continue;
                }

                potentials[i] += dist[i];
            }

            // Find max flow on augmenting path
            int f = K - total_flow;
            int curr = sink;
            while (curr != src) {
                int prev = parent[curr].first;
                int idx = parent[curr].second;

                f = min(f, adj[prev][idx].cap);
                curr = prev;
            }

            total_flow += f;

            // Adjust capacities according to
            // altered flow
            curr = sink;
            while (curr != src) {
                int prev = parent[curr].first;
                int idx = parent[curr].second;
                int idx_rev = adj[prev][idx].rev_idx;

                // Adjust capacities
                adj[prev][idx].cap -= f;
                adj[curr][idx_rev].cap += f;

                // Adjust flows
                flow[prev][curr] += f;
                flow[curr][prev] = max(flow[curr][prev] - f, 0);

                total_cost += f * adj[prev][idx].cost;

                curr = prev;
            }
        }

        return {total_flow, total_cost, move(flow)};
    }

private:
    /**
     * Dijkstra algorithm
     *
     * @param src Source node
     * @param adj Adjacency list
     * @param dist Distance array
     * @param parent Parent array
     * @param potentials Potentials
     */
    void dijkstra(
        int src,
        vector<vector<ResidualEdge>> & adj,
        vector<ll> & dist,
        vector<pair<int, int>> & parent,
        vector<ll> & potentials
    ) {
        dist.assign(n, INF);
        parent.assign(n, {-1, -1});

        dist[src] = 0;

        priority_queue<
            pair<ll, int>,
            vector<pair<ll, int>>,
            greater<pair<ll, int>>
        > pq;
        pq.push({0, src});

        while (! pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            // Already processed better path
            if (d > dist[u]) {
                continue;
            }

            for (int i = 0; i < adj[u].size(); ++i) {
                auto & e = adj[u][i];

                if (e.cap <= 0) {
                    continue;
                }

                ll alt = dist[u] + ((ll) e.cost) + potentials[u] - potentials[e.to];
                if (alt >= dist[e.to]) {
                    continue;
                }

                dist[e.to] = alt;
                parent[e.to] = {u, i};
                pq.push({alt, e.to});
            }
        }
    }

    /**
     * Compute potentials for the dijkstra algorithm
     * using Bellman-Ford.
     * 
     * @param adj Adjacency list
     * @return Potentials
     */
    vector<ll> compute_potentials(
        int src,
        vector<vector<ResidualEdge>> & adj
    ) {
        BellmanFord::Graph graph(n, edges.size());
        for (auto & e : edges) {
            graph.add_edge(e.u, e.v, e.cost);
        }

        BellmanFord::BellmanFord bf(move(graph), src);
        bf.run();

        return bf.dist;
    }
};

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    EdgeList edges;
    for (int i = 0; i < m; ++i) {
        int u, v, cap, cost;
        cin >> u >> v >> cap >> cost;

        edges.push_back({u, v, cost, cap});
    }

    SSP ssp(n, edges);
    auto res = ssp.min_cost_max_flow(s, t);

    cout << res.flow << " " << res.cost << "\n";
    cout << flush;

    return 0;
}