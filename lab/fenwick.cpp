#include <iostream>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-04
 */

/**
 * Implementation according to: https://cp-algorithms.com/data_structures/fenwick.html
 *
 * Fenwick tree allows updates and range queries in O(log n) time.
 * It achieves this by storing partial sums in a binary tree structure.
 * Queries are done by summing over partial sums, which can be done in O(log n) time.
 *
 * As for data structures, we can use a simple array to store the partial sums.
 *
 * Note that this implementation is 1-indexed! According to the spec sum(0) should
 * be understood as an empty sum, using 1-indexing makes this easier.
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
     * Get sum of elements of first i elements!
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