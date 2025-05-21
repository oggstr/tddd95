#include <iostream>
#include <vector>
#include <limits>
#include <queue>

namespace BellmanFord {

using namespace std;
using ll = long long int;
using llu = long long unsigned;

const ll INF = numeric_limits<ll>::max() / 2;

/**
 * Models a directed edge
 */
class Edge
{
public:

    /**
     * To node
     */
    int v;

    /**
     * Weight (time)
     */
    int w;

    Edge(int v, int w) : v(v), w(w) {}
};

class Graph
{
public:

    /**
     * Node count
     */
    int N;

    /**
     * Edge count
     */
    int M;

    /**
     * Adjacency list
     */
    vector<vector<Edge>> adj;

    /**
     * Constructor
     * @param n Number nodes
     * @param m Number edges
     */
    Graph(int n, int m) : N(n), M(m)
    {
        adj.resize(N, vector<Edge>());
    }

    /**
     * Add edge from u to v with weight w
     * @param u Node u
     * @param v Node v
     * @param w Weight
     */
    void add_edge(int u, int v, int w)
    {
        adj[u].push_back(Edge(v, w));
    }

    /**
     * Get neighbors of u
     * @param u Node u
     * @return Vector neighbors
     * @throws std::out_of_range if u is out of range
     */
    const vector<Edge> & neighbors(int u) const
    {
        if (u < 0 || u >= N) {
            throw std::out_of_range("Node out of range");
        }

        return adj[u];
    }
};

class BellmanFord
{
public:
    /**
     * Graph
     */
    Graph G;

    /**
     * Distance from start to each node
     */
    vector<ll> dist;

    /**
     * Previous node in the path
     */
    vector<int> prev;

    /**
     * Start node
     */
    int start;

    /**
     * Negative cycle
     */
    vector<int> negative_cycle;

    BellmanFord(Graph && G, int start) : G(move(G)), start(start)
    {
        dist.resize(this->G.N, INF);
        prev.resize(this->G.N, -1);
    }

    /**
     * Run Bellman-Ford algorithm
     */
    void run()
    {
        dist[start] = 0;

        // Time complexity O(n * m)
        for (int i = 0; i < G.N-1; ++i) {

            // These two loops involve m steps
            for (int u = 0; u < G.N; ++u) {
                for (auto e : G.neighbors(u)) {

                    // Not reached
                    if (dist[u] >= INF) {
                        continue;
                    }

                    ll alt = dist[u] + e.w;
                    // No improvement
                    if (alt >= dist[e.v]) {
                        continue;
                    }

                    dist[e.v] = max(-INF, alt);
                    prev[e.v] = u;
                }
            }
        }

        // Check for negative cycles
        // If any node is updated, there exists a negative cycle
        bool changed = false;
        vector<int> neg;
        for (int u = 0; u < G.N; ++u) {
            for (auto e : G.neighbors(u)) {

                // Not reached
                if (dist[u] >= INF) {
                    continue;
                }

                ll alt = dist[u] + e.w;
                // No improvement
                if (alt >= dist[e.v]) {
                    continue;
                }

                dist[e.v] = max(-INF, alt);
                prev[e.v] = u;
                changed = true;
                neg.push_back(e.v);
            }
        }

        if (!changed) {
            return;
        }

        // Mark nodes reachable from the negative cycle
        // BFS
        while (!neg.empty()) {
            int u = neg.back();
            neg.pop_back();
            dist[u] = -INF;

            for (auto e : G.neighbors(u)) {
                if (dist[e.v] == -INF) {
                    continue;
                }

                neg.push_back(e.v);
            }
        }
    }

    /**
     * Get shortest path from start to end
     *
     * @param end End node
     * @return Vector path
     * @throws std::runtime_error if no path exists
     */
    vector<int> shortest_path(int end) const
    {
        vector<int> path;
        for (int v = end; v != -1; v = prev[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());

        // This means that start and end
        // nodes are not connected
        if (path.size() == 0 || path[0] != start) {
            throw std::runtime_error("No path found");
        }

        return path;
    }

    /**
     * Get distance from start to end
     *
     * @param end End node
     * @return Distance
     * @note INF if no path exists and -INF reachable from negative cycle
     */
    ll get_distance(int end) const
    {
        if (end < 0 || end >= G.N) {
            throw std::out_of_range("Node out of range");
        }

        return dist[end];
    }
};
};

