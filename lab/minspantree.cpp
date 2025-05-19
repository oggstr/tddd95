#include <iostream>
#include <vector>
#include <algorithm>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-19
 */

/** Implementation of Kruskal's algorithm using union-find
 * reference: https://cp-algorithms.com/graph/mst_kruskal_with_dsu.html
 *
 * Algorithm:
 *
 * The algorithm is quite simple. Start with an empty graph, then always
 * add the cheapest edge that doesn't create a cycle.
 *
 * We do this by sorting edges by weight, iterating over them and checking
 * if two nodes belong in the same set using union-find.
 *
 * Time complexity:
 *
 * - O(m log m): Sorting edges
 * - O(m * α(n)): Union-find operations
 *
 * Where m is the number of edges and n is the number of nodes. Since n < 20 000 in this problem,
 * α(n) < 4 - essentially constant. The dominating factor is therefore the sorting - so in total
 * we have:
 *  O(m log m).
 * 
 * Space complexity:
 * 
 *  O(n + m)
 * Edges are stored using a vector, and union-find uses two arrays of size n.
 */

using namespace std;

class UnionFind {
private:
    int *parent;
    int *rank;
    int size;

public:
    UnionFind(int n)
    {
        size   = n;
        parent = new int[n];
        rank   = new int[n];

        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    ~UnionFind()
    {
        delete[] parent;
        delete[] rank;
    }

    /**
     * Find set representative for element x
     * @param x element
     * @return set representative
     */
    int find(int x)
    {
        if (parent[x] != x) {
            return parent[x] = find(parent[x]);
        }

        return parent[x];
    }

    /**
     * Join (union) two sets
     * @param x first set
     * @param y second set
     */
    void join(int x, int y)
    {
        int rx = find(x);
        int ry = find(y);

        if (rx == ry) {
            return;
        }

        if (rank[rx] < rank[ry]) {
            swap(rx, ry);
        }

        parent[ry] = rx;

        if (rank[rx] == rank[ry]) {
            rank[rx]++;
        }
    }

    /**
     * Check if two sets are the same
     * @param x first set
     * @param y second set
     */
    bool same(int x, int y)
    {
        return find(x) == find(y);
    }
};

class Edge
{
public:
    /**
     * Start node, end node, weight
     */
    int u, v, w;

    Edge(int u, int v, int w) : u(u), v(v), w(w) {}

    /**
     * Compare less than
     *
     * @param other Other edge
     * @return True if less
     */
    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
};

class MST
{
public:
    /**
     * Set for Kruskal's algorithm
     */
    UnionFind uf;

    /**
     * Graph
     */
    vector<Edge> edges;

    /**
     * Minimum spanning tree
     */
    vector<Edge> mst;

    /**
     * Number nodes
     */
    int n;

    /**
     * Cost
     */
    int cost = 0;

    /**
     * Constructor
     *
     * @param edges Edges
     * @param n Number of nodes
     */
    MST(vector<Edge> edges, int n) : edges(edges), uf(UnionFind(n)), n(n) {
        mst.reserve(n-1);
    }

    void run()
    {
        // Sort edges
        // Time: O(m log m)
        sort(edges.begin(), edges.end());

        // Kruskal's algorithm
        for (const auto &edge : edges) {

            // We're done if we have n-1 edges
            if (mst.size() == n - 1) {
                break;
            }

            int u = edge.u;
            int v = edge.v;
            int w = edge.w;

            if (! uf.same(u, v)) {
                mst.push_back(edge);
                uf.join(u, v);
                cost += w;
            }
        }
    }

    /**
     * Get cost
     * 
     * @return Cost
     */
    int get_cost()
    {
        return cost;
    }

    /**
     * Get minimum spanning tree
     * 
     * @return Tree
     */
    vector<Edge> get_mst()
    {
        return mst;
    }
};

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;

    while (1) {
        cin >> n >> m;
        if (n == 0 && m == 0) {
            break;
        }

        // Parse input
        vector<Edge> edges;
        int u, v, w;
        for (int i = 0; i < m; ++i) {
            cin >> u >> v >> w;

            if (u < v) {
                edges.push_back(Edge(u, v, w));
            } else {
                edges.push_back(Edge(v, u, w));
            }
        }

        // Run Kruskal's
        MST mst(edges, n);
        mst.run();

        // Get solution
        int cost = mst.get_cost();
        vector<Edge> mst_edges = mst.get_mst();

        // MST must have exactly n-1 edges
        if (mst_edges.size() != n - 1) {
            cout << "Impossible\n";
            continue;
        }

        cout << cost << '\n';

        // Transform mst so that we can print:
        // "The edges should be output so that and should be listed in the lexicographic order on pairs of integers"
        sort(mst_edges.begin(), mst_edges.end(), [](const Edge &a, const Edge &b) {
            if (a.u == b.u) {
                return a.v < b.v;
            }
            return a.u < b.u;
        });

        for (const auto & e : mst_edges) {
            cout << e.u << " " << e.v << '\n';
        }
    }
}