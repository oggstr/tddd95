#include <iostream>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-04
 */

/**
 * Implementation of Fenwick Tree to compute prefix sums.
 * Reference: https://cp-algorithms.com/data_structures/fenwick.html
 *
 * @note that this implementation is 1-indexed! According to the spec sum(0) should
 * be understood as an empty sum, using 1-indexing makes this easier.
 *
 * Algorithm
 *
 * A Fenwick Tree uses bit manipulation to store partial sums in a tree-like structure.
 * Some examples:
 * - index 0110 holds the sum [5, 6]
 * - index 0100 holds the sum [1, 4]
 * - index 1000 holds the sum [1, 8]
 * (Note the 1-indexing, 0 is a dummy node)
 *
 * The structure supports two operations. First, we may query the sum of
 * the first i elements. Second, we may add (or update) a value at index i.
 *
 * Query
 * Summing works by moving from the queried index i to the next index
 * by subtracting the lowest set bit of i (i & -i). For example the query
 * sum(7) = sum(0111) = A[0111] + A[0110] + A[0100]
 *
 * Update
 * To update the value at index i, we add the value to A[i] and then
 * propagate the change to all indices that include i in their range.
 * Effectively, we move opposite to that of the query operation.
 *
 * Time complexity:
 * - Update: O(log n)
 * - Query: O(log n)
 *
 * Where n is the number of elements in the Fenwick tree.
 *
 * For the Kattis problem in question, let n be the number of elements,
 * and m be the number of queries. The total time complexity is
 * O(n log n + m log n) = O((n + m) log n).
 * 
 * Space complexity:
 * - O(n) for the tree array
 * 
 * Data structures:
 * - Array to store partial sums indexed by the Fenwick tree structure
 */

/* Complementary work  

# How does a Fenwick Tree improve on the naive O(N) approach? What partial sums are stored and why?

A Fenwick Tree allows both updates and queries to be performed in O(log n) time.
Storing partial sums effectively balances the time complexity between updates and queries.

A naive approach would require O(1) for updates and O(n) for queries (or vice verse if we store an array of sums).
This means for m queries, the total time complexity would be O(nm).

*/

#define ll long long int

using namespace std;

class FenwickTree {
public:

    ll n;
    ll * arr;

    FenwickTree(ll n): n(n) {
        arr = new ll[n+1]();
    }

    ~FenwickTree() {
        delete[] arr;
    }

    /**
     * Add value to index i
     * @param i index
     * @param val value
     */
    void add(ll i, ll val)
    {
        i += 1; // 1-indexed
        for (; i <= this->n; i += i & (-i)) {
            this->arr[i] += val;
        }
    }

    /**
     * Get sum of elements of first i elements
     * @param i amount of elements
     * @return sum
     */
    ll sum(ll i)
    {
        ll res = 0;
        for(; i > 0; i -= i & (-i)) {
            res += this->arr[i];
        }

        return res;
    }
};

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, Q;
    cin >> N >> Q;
    FenwickTree ft(N);

    char op;
    ll x, y;
    for (ll i = 0; i < Q; ++ i) {
        cin >> op;

        switch (op)
        {
        case '+':
            cin >> x >> y;
            ft.add(x, y);
            break;

        case '?':
            cin >> x;
            cout << ft.sum(x) << '\n';
            break;
        default:
            cerr << "Error: Unkown operation " << op << '\n';
            break;
        }
    }

    cout << flush;
    return 0;
}