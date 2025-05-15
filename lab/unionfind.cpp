#include <iostream>
#include <algorithm>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-02
 */

/**
 * Simple union-find implementation using path compression and union by rank.
 * This allows an average time complexity of O(1).
 *
 * The data structure itself is quite simple. Each set has a representative.
 * Initially, each element is its own representative. To join sets we simply
 * need to find the representative of each set and set the representative
 * of one set to the representative of the other. To find a representative
 * we simply follow a chain of 'parent' pointers until we reach the root of the tree.
 *
 * The data itself can be stored using a simple array. Where each index corresponds
 * to an element, and the value at an index is the parent of that element. (On top of this,
 * we also store a rank array for union optimization.)
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

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    UnionFind uf(N);

    char op;
    int x, y;
    for (int i = 0; i < M; i++) {
        cin >> op >> x >> y;

        switch (op)
        {
        case '=':
            uf.join(x, y);
            break;
        
        case '?':
            cout << (uf.same(x, y) ? "yes" : "no") << '\n';
            break;
        }
    }

    return 0;
}