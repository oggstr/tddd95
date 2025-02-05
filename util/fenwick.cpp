/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-30
 */

#include <vector>

class FenwickTree
{

public:

    int n;
    std::vector<int> arr;

    FenwickTree(int n): n(n), arr(n+1, 0) {}

    void update(int i, int val)
    {
        for (; i <= this->n; i += i & (-i)) {
            this->arr[i] += val;
        }
    }

    int query(int i)
    {
        int res = 0;
        for(; i > 0; i -= i & (-i)) {
            res += this->arr[i];
        }

        return res;
    }

};