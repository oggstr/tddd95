#include <iostream>
#include <vector>
#include <limits>
#include <queue>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-16
 */

/**
 * Implementation of Dijkstra's algorithm with time tables
 *
 * The only difference between this implementation and shortestpath1.cpp
 * is that we filter and adjust neighbor edge weights based on the current time.
 * See the neighbors() function for details.
 *
 * @note Assumes that no edge has negative weight!
 *
 * Algorithm:
 *
 * Standard Dijkstra's algorithm using a priority queue.
 * We start from the source node and explore all neighbors.
 * If we find a path to a neighbor that is shorter than what we've
 * already found, we update the distance and add the neighbor to the queue.
 *
 * When we reach the end node, we backtrack to find the shortest path.
 *
 *
 * Time complexity:
 *
 * O((n + m) * log n)
 * where n is the number of nodes, and m is the number of edges
 * 
 * Which is the same as before. We've essentially just added a setp of O(n)
 * when getting the neighbors of a node.
 *
 *
 * Space complexity:
 *
 * - O(n + m) for the graph
 * - O(n) for the distance and previous node arrays
 * - O(m) for the priority queue
 * In total: O(n + m)
 * 
 * 
 * Data structures:
 *
 * - Adjacency list using vectors
 * - Priority queue for the Dijkstra's algorithm
 */

using namespace std;

using ll = long long int;
using llu = long long unsigned;

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

    /**
     * Time this edge was be
     * traversed
     */
    int t0;

    /**
     * Time step when this edge
     * may be traversed.
     *
     * @note if 0 this edge may only
     * be traversed at time t0!
     */
    int p;

    Edge(int v, int w, int t0, int p) : v(v), w(w), t0(t0), p(p) {}
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
     * Infinity
     */
    static constexpr int INF = numeric_limits<int>::max();

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
    void add_edge(int u, int v, int w, int t0, int p )
    {
        adj[u].push_back(Edge(v, w, t0, p));
    }

    /**
     * Get neighbors of u
     * @param u Node u
     * @param t time
     * @return Vector neighbors
     * @throws std::out_of_range if u is out of range
     */
    vector<Edge> neighbors(int u, int t) const
    {
        if (u < 0 || u >= N) {
            throw std::out_of_range("Node out of range");
        }

        vector<Edge> neighbors;
        for (auto & e : adj[u]) {
            Edge e2 = e;

            // Wait time until t0
            if (t <= e.t0) {
                e2.w += (e.t0 - t);

            // Too late, this edge cannot be traversed
            } else if (e.p == 0) {
                continue;

            // Wait until next time this edge can be traversed
            } else {
                // Compute potential wait time and adjust the weight:
                //      (t - e.t0) % e.p                :  amount of time we've overrun departure
                //      (e.p - (t - e.t0) % e.p)        :  inverse, amount of time we have to wait
                //      (e.p - (t - e.t0) % e.p) % e.p  :  correct for 0 wait time
                int time_offset = (e.p - (t - e.t0) % e.p) % e.p;
                e2.w += time_offset;
            }

            neighbors.push_back(e2);
        }

        return neighbors;
    }
};

class Dijkstra
{
public:

    /**
     * Graph
     */
    Graph G;

    /**
     * Distance from start to each node
     */
    vector<int> time;

    /**
     * Previous node in the path
     */
    vector<int> prev;

    /**
     * Start node
     */
    int start;

    /**
     * Constructor
     * @param G Graph
     * @param start Start node
     */
    Dijkstra(Graph && G, int start) : G(move(G)), start(start)
    {
        time.resize(this->G.N, G.INF);
        prev.resize(this->G.N, -1);
    }

    /**
     * Run Dijkstra's algorithm
     */
    void run()
    {
        time[start] = 0;

        // Priority queue with pair <distance, node>
        // Elements priority sorted by distance
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        pq.push({0, start});

        while (!pq.empty()) {
            auto [t, u] = pq.top();
            pq.pop();

            // Skip if we've already seen a shorter path
            if (t > time[u]) {
                continue;
            }

            for (auto e : G.neighbors(u, t)) {
                int alt = time[u] + e.w;

                if (alt < time[e.v]) {
                    time[e.v] = alt;
                    prev[e.v] = u;
                    pq.push({alt, e.v});
                }
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
     * Get time from start to end
     *
     * @param end End node
     * @return Time
     * @note Returns INF if no path exists
     */
    int get_time(int end) const
    {
        return time[end];
    }
};

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q, s;
    while (1) {
        cin >> n >> m >> q >> s;
        if (n == 0 && m == 0 && q == 0 && s == 0) {
            break;
        }

        Graph G(n, m);

        int u, v, t0, p, d;
        for (int i = 0; i < m; i++) {
            cin >> u >> v >> t0 >> p >> d;
            G.add_edge(u, v, d, t0, p);
        }

        Dijkstra dijkstra(move(G), s);
        dijkstra.run();

        for (int i = 0; i < q; i++) {
            cin >> u;

            if (dijkstra.get_time(u) == Graph::INF) {
                cout << "Impossible\n";
            } else {
                cout << dijkstra.get_time(u) << "\n";
            }
        }
    }

    cout << flush;
    return 0;
}