#include <iostream>
#include <vector>
#include <cmath>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-24
 */

/**
 * Eulerian path finder
 * reference: https://cp-algorithms.com/graph/euler_path.html
 *
 * Algorithm:
 * We first check if the graph has an Euler path by
 * checking the in-degree and out-degree of each node.
 * If it does, we perform a DFS search to find it.
 *
 * Time complexity:
 * O(m)
 * Where n is nodes and m is edges.
 *
 * Space complexity:
 * O(m)
 * 
 * Data structures:
 * - Adjacency list using vectors
 * - Vector for in-degree and out-degree
 * 
 * Usage:
 * Supports multi graphs, i.e. multiple edges
 * between the same nodes.
 */

using namespace std;
using adj_list = vector<vector<int>>;

class EulerPath {
public:
    /**
     * Directed graph
     */
    adj_list adj;

    /**
     * In-degree of nodes
     */
    vector<int> deg_in;

    /**
     * Out-degree of nodes
     */
    vector<int> deg_out;

    /**
     * Node count
     */
    int n = 0;

    /**
     * Edge count
     */
    int m = 0;

    EulerPath(adj_list adj) : adj(adj), n(adj.size()) {
        deg_in.resize(n, 0);
        deg_out.resize(n, 0);

        for (int u = 0; u < n; ++u) {
            for (auto v : adj[u]) {
                deg_out[u]++;
                deg_in[v]++;
            }

            // Count edges
            m += deg_out[u];
        }
    }

    /**
     * Find Euler path
     *
     * @return {exists, path}
     */
    pair<bool, vector<int>> euler_path()
    {
        // Trivial case
        if (adj.empty()) {
            return {true, {}};
        }

        if (! has_euler_path()) {
            return {false, {}};
        }

        vector<int> path;
        vector<int> deg = deg_out;

        find_path(start_node(), path, deg);

        if (path.size() != m + 1) {
            return {false, {}};
        }

        // Reverse path to get correct order
        reverse(path.begin(), path.end());

        return {true, path};
    }

private:

    /**
     * Find Euler path recursively (DFS)
     *
     * @param u Current node
     * @param path Path
     * @param deg Degree - used to index 
     */
    void find_path(
        int u,
        vector<int>& path,
        vector<int>& deg
    ) {
        while (deg[u] > 0) {
            // Index using degree, allows
            // the same edge to exist multiple times
            find_path(adj[u][--deg[u]], path, deg);
        }

        path.push_back(u);
    }

    /**
     * Check if graph has Euler path
     *
     * @return {true} if graph has Euler path,
     * otherwise {false}
     */
    bool has_euler_path()
    {
        int s = 0;
        int e = 0;

        for (int u = 0; u < n; ++u) {

            if (abs(deg_in[u] - deg_out[u]) > 1) {
                return false;
            }

            if (deg_out[u] - deg_in[u] == 1) {
                s++;
            } 

            if (deg_in[u] - deg_out[u] == 1) {
                e++;
            }
        }

        return (s == 0 && e == 0) || (s == 1 && e == 1);
    }

    /**
     * Find start node
     *
     * @note This function should be called after
     * has_euler_path() returns true. Otherwise,
     * it may return an invalid node.
     *
     * @return {start node}
     */
    int start_node() {
        int s = -1;
        for (int u = 0; u < n; ++u) {

            // If u has larger out-deg than in-deg,
            // we must start from u
            if (deg_out[u] - deg_in[u] == 1) {
                s = u;
                break;
            }

            // u has out going edge, and is good
            // enough to start from
            if (deg_out[u] > 0) {
                s = u;
            }
        }

        return s;
    }
};

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n, m;
    while (1) {
        cin >> n >> m;
        if (n == 0 && m == 0) break;

        adj_list adj(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
        }

        EulerPath euler(adj);
        auto [exists, path] = euler.euler_path();

        if (!exists) {
            cout << "Impossible\n";
            continue;
        }

        for (auto v : path) {
            cout << v << " ";
        }
        cout << "\n";
    }

    cout << flush;
    return 0;
}