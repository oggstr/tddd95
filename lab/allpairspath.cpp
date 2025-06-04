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
 * reference: https://cp-algorithms.com/graph/all-pair-shortest-path-floyd-warshall.html
 *
 * @note There are no requirements on path reconstruction here. Therefore
 * we skip a proper representation of the graph entirely. Instead we just
 * use a distance matrix and insert new edges directly into it - thereby also
 * performing the first step of the algorithm for free.
 *
 * Algorithm:
 *
 * Floyd-Warshall algorithm with support for negative weights and cycles.
 * The algorithm uses a dynamic programming approach. Throughout the algorithm,
 * we maintain a distance matrix d where d[i][j] is the current shortest distance
 * from i to j, given that we can use any intermediate nodes up to some k.
 *
 * Initialization
 * 1. Start by initializing a distance matrix with infinity. (Performed by constructor)
 * 2. Set the distance from each node to itself to 0. (Performed by constructor)
 * 3. For each edge (u, v) with weight w, set d[u][v] = min(d[u][v], w). (Performed by addEdge)
 *
 * Core alg (Performed by run())
 * 4. For each k from 0 to n-1, iterate over all pair of nodes (i, j)
 * 5. Update the distance matrix: d[i][j] = min(d[i][j], d[i][k] + d[k][j])
 *    Essentially, check if we can improve the distance from i to j by introducing a new node k.
 *
 * Negative cycle detection after running the algorithm
 * 5. For each k from 0 to n-1, iterate over all pair of nodes (i, j)
 * 6. Check if the distance between i and j can still be improved through k
 * 7. If it can, it's part of a negative cycle, so set d[i][j] = -INF
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
     * Distance matrix
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