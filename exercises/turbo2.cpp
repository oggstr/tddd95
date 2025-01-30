/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-30
 */

#include <iostream>
#include <vector>

using namespace std;

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

int N;
int lookup [100002] = {0};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;

    FenwickTree ft(N);

    int val;
    for (int i = 1; i <= N; ++i) {
        cin >> val;
        lookup[val] = i;
        ft.update(i, 1);
    }


    for (int i = 1; i <= N; ++i) {

        if (i % 2 == 1) {
            int num = i/2 + 1; 
            int index = lookup[num];

            // cout << "num = " << num << " index = " << index << '\n';
            // cout << "ft.query(index) = " << ft.query(index) << '\n';

            cout << ft.query(index) - 1 << '\n';
            ft.update(index, -1);
        } else {
            int num = N - (i/2) + 1;
            int index = lookup[num];

            // cout << "num = " << num << " index = " << index << '\n';
            // cout << "ft.query(N) = " << ft.query(N) << " ft.query(index) = " << ft.query(index) << '\n';

            cout << ft.query(N) - ft.query(index) << '\n';
            ft.update(index, -1);
        }
    }

    cout << flush;

    return 0;
}