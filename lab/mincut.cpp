#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <set>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-19
 */

 /**
  * Implementation of the Edmonds-Karp and BFS to find min cut
  * reference: https://cp-algorithms.com/graph/edmonds_karp.html#max-flow-min-cut-theorem
  *
  * Algorithm:
  * 
  * Exact same Edmonds-Karp as in max flow. The only difference
  * is that the residual graph is now returned, and the min_cut function
  * performs a BFS on the residual graph to find the min cut.
  *
  * Time complexity:
  * - O(n * m^2)
  * - O(n * m) for the BFS
  * Total: still O(n * m^2)
  * 
  * where n is the number of nodes, and m is the number of edges
  * 
  * Space complexity:
  * - O(n * m) (for the graph)
  * - O(n^2) (for the capacity and flow matrices)
  *
  * Note that this implementation does not deduplicate edges.
  * Therefore, the space complexity is best described as O(n * m).
  * 
  * (Where n is nodes and m is edges)
  *
  * Data structures:
  * - Adjacency list using vectors
  * - Capacity matrix using vectors
  * - Flow matrix using vectors
  *
  * Usage:
  *
  * An EdmondsKarp object may be reused for multiple max flow queries
  * as it doesn't modify it's underlying graph.
  */

using namespace std;

const int INF = numeric_limits<int>::max();

/**
 * @note The capacity is directed,
 * but the graph is undirected.
 */
class Graph {
public:
    /**
     * Adjacency list
     */
    vector<vector<int>> adj;

    /**
     * Capacity matrix
     */
    vector<vector<int>> capacity;

    /**
     * Number of nodes
     */
    int n;

    /**
     * Constructor
     * @param n Number of nodes
     */
    Graph(int n) : n(n) {
        adj.resize(n);
        capacity.resize(n, vector<int>(n, 0));
    }

    /**
     * Move constructor
     */
    Graph(Graph && g) : n(g.n), adj(move(g.adj)), capacity(move(g.capacity)) {}

    /**
     * Add edge to the graph
     * @note The capacity is directed,
     * but the graph is undirected.
     */
    void addEdge(int u, int v, int cap)
    {
        adj[u].push_back(v);
        adj[v].push_back(u); // Back edge for BFS
        capacity[u][v] = cap;
    }
};

/**
 * Ford-Fulkerson using BFS
 */
class EdmondsKarp {
public:

    /**
     * Graph
     */
    Graph g;

    /**
     * Nodes
     */
    int n;

    /**
     * Move constructor
     * @param g Graph
     */
    EdmondsKarp(Graph && g) : n(g.n), g(move(g)) {}

    /**
     * Max flow
     * @param source Source node
     * @param sink Sink node
     * @return Max flow and flow matrix
     */
    pair<int, vector<vector<int>>> max_flow(
        int source,
        int sink,
        vector<vector<int>> & residual_cap
    ) {
        int flow = 0;
        int new_flow;
        vector<int> parent(n);
        
        
        // Copy over capcity matrix,
        // avoiding modifying the original         
        auto cap = g.capacity;

        // Track flows
        vector<vector<int>> flows(n, vector<int>(n, 0));
        
        while (new_flow = bfs(source, sink, parent, cap)) {
            flow += new_flow;

            int curr = sink;
            while (curr != source) {
                int prev = parent[curr];

                // Adjust capacity
                cap[prev][curr] -= new_flow;
                cap[curr][prev] += new_flow;

                // Adjust flows
                flows[prev][curr] += new_flow;
                flows[curr][prev] -= new_flow;

                curr = prev;
            }
        }

        residual_cap = move(cap);
        return {flow, move(flows)};
    }

    /**
     * Find minimum cut
     *
     * Runs max flow, followed by BFS search to find
     * reachable nodes.
     * 
     * @param source Source node
     * @param sink Sink node
     * @return Set of nodes in min cut
     */
    set<int> min_cut(int source, int sink)
    {
        vector<vector<int>> residual_cap;
        max_flow(source, sink, residual_cap);

        set<int> cut;
        queue<int> q;

        q.push(source);
        cut.insert(source);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : g.adj[u]) {

                // Sink not in cur by definition
                if (v == sink) {
                    continue;
                }

                // Already visited
                if (cut.find(v) != cut.end()) {
                    continue;
                }

                // No capacity left
                if (residual_cap[u][v] <= 0) {
                    continue;
                }

                q.push(v);
                cut.insert(v);
            }
        }

        return cut;
    }

private:
    /**
     * BFS to find augmenting path
     * @param source Source node
     * @param sink Sink node
     * @param parent Parent array
     * @param cap Capacity matrix
     * @return Flow of path
     */
    int bfs(int source, int sink, vector<int>& parent, vector<vector<int>>& cap)
    {
        fill(parent.begin(), parent.end(), -1);

        parent[source] = -2;

        vector<bool> visited(n, false);
        visited[source] = true;

        queue<pair<int, int>> q;
        q.push({source, INF});

        while (! q.empty()) {
            int curr = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : g.adj[curr]) {
                if (visited[next]) {
                    continue;
                }

                if (parent[next] != -1 || cap[curr][next] == 0) {
                    continue;
                }

                parent[next] = curr;
                int new_flow = min(flow, cap[curr][next]);
                if (next == sink) {
                    return new_flow;
                }

                q.push({next, new_flow});
            }
        }

        return 0;
    }
};


int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    Graph g(n);
    vector<pair<int, int>> edges(m); // Track original edges
    for (int i = 0; i < m; ++i) {
        int u, v, cap;
        cin >> u >> v >> cap;

        g.addEdge(u, v, cap);
        edges[i] = {u, v};
    }


    EdmondsKarp ek(move(g));
    auto cut = ek.min_cut(s, t);

    cout << cut.size() << "\n";

    for (auto u : cut) {
        cout << u << "\n";
    }

    cout << flush;
    return 0;
}