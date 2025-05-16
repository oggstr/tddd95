#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-16
 */

/**
 * Floyd-Warshall algorithm
 *
 * @note There are no requirements on path reconstruction here. Therefore
 * we skip a proper representation of the graph entirely. Instead we just
 * use a distance matrix and insert new edges directly into it - thereby also
 * performing the first step of the algorithm for free.
 *
 * Algorithm:
 *
 * Standard Floyd-Warshall algorithm with support for negative weights and cycles.
 * In the constructor we initialize the distance matrix with INF and 0 on the diagonal.
 * Adding an edge is done directly by setting the distance matrix entry to the edge weight.
 * The run() method then performs the standard Floyd-Warshall algorithm.
 * 
 * reference: https://cp-algorithms.com/graph/all-pair-shortest-path-floyd-warshall.html
 *
 * Time complexity:
 *
 * O(n^3)
 * where n is the number of nodes
 *
 * Space complexity:
 *
 * O(n^2)
 * where n is the number of nodes
 * 
 */

using namespace std;

const int INF = numeric_limits<int>::max();

class FloydWarshall
{
public:
    /**
     * Number nodes
     */
    int n;

    /**
     * Adjacency/distance matrix
     */
    vector<vector<int>> dist;

    /**
     * Constructor
     *
     * @param n Number nodes
     */
    FloydWarshall(int n) : n(n), dist(n, vector<int>(n, INF))
    {
        for (int i = 0; i < n; i++) {
            dist[i][i] = 0;
        }
    }

    /**
     * Add edge
     *
     * @note, must be used before run() is called!
     *
     * @param u Start node
     * @param v End node
     * @param w Weight
     */
    void addEdge(int u, int v, int w)
    {
        // We're not concerned with duplicate edges,
        // we just want the minimum. On top of that, this
        // also avoids an edge case with self-loops.
        dist[u][v] = min(dist[u][v], w);
    }

    /**
     * Run Floyd-Warshall algorithm
     */
    void run()
    {
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {

                    // Check that path exists
                    if (dist[i][k] < INF && dist[k][j] < INF) {

                        // Update if better
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]); 
                    }
                }
            }
        }

        /* From reference - https://cp-algorithms.com/graph/all-pair-shortest-path-floyd-warshall.html:

        "This can be done in the following way: let us run the usual Floyd-Warshall algorithm for a given graph.
        Then a shortest path between vertices $i$ and $j$ does not exist, if and only if,
        there is a vertex $t$ that is reachable from $i$ and also from $j$, for which $d[t][t] < 0$."

        In other words, if there is a negative cycle reachable from both $i$ and $j$,
        then the distance between $i$ and $j$ is undefined/negative INF.
        */
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dist[i][k] < INF && dist[k][j] < INF && dist[k][k] < 0) {
                        dist[i][j] = -INF;
                    }
                }
            }
        }
    }
};

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;

    while (1) {
        cin >> n >> m >> q;
        if (n == 0 && m == 0 && q == 0) {
            break;
        }

        FloydWarshall fw(n);

        int u, v, w;
        for (int i = 0; i < m; i++) {
            cin >> u >> v >> w;
            fw.addEdge(u, v, w);
        }

        fw.run();

        for (int i = 0; i < q; i++) {
            cin >> u >> v;

            if (fw.dist[u][v] == INT_MAX) {
                cout << "Impossible\n";
                continue;
            }

            if (fw.dist[u][v] == -INF) {
                cout << "-Infinity\n";
                continue;
            }

            cout << fw.dist[u][v] << "\n";
        }
    }
}