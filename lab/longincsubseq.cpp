#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-04
 */

/**
 * Implementation of: https://cp-algorithms.com/sequences/longest_increasing_subsequence.html
 * (Including reconstruction of the sequence)
 *
 * Algorithm:
 * Works though dynamic programming and binary search.
 * At dp[l] we store the smallest element of subsequence of length l.
 * At every point in the iteration we find the largest l such that dp[l-1] < arr[i].
 * If we find such an l, we update dp[l] to arr[i].
 *
 * The final result is the largest l where dp[l] < oo.
 *
 * Besides this we also track predecessors of each element in the sequence.
 * This allows us to reconstruct the sequence in O(n) time.
 *
 * Data structures:
 * - dp: array of integers
 * - p: array of integers
 * - prev: array of integers 
 * 
 * Complexity:
 * Time: O(n log n)
 * Space: O(n)
 */

#define SZ 1000000

const int oo = std::numeric_limits<int>::max();

using namespace std;

/**
 * Longest Increasing Subsequence
 * @param arr array of integers
 * @return list of indices of subsequence
 */
vector<int> lis(vector<int> const& arr) {
    int n = arr.size();

    // dp[i] = smallest elem of increasing subsequence of length i
    vector<int> dp(n+1, oo);
    dp[0] = -oo;
    
    // p[i] = index in arr of element that ends subsequence of length i
    vector<int> p(n+1, -1);
    
    // prev[i] = predecessor of arr[i] in the subsequence
    vector<int> prev(n, -1);

    // Time: O(n log n)
    for (int i = 0; i < n; i++) {
        // Bin search for largest position l where dp[l-1] < arr[i]
        int l = upper_bound(dp.begin(), dp.end(), arr[i]) - dp.begin();

        if (dp[l-1] < arr[i] && arr[i] < dp[l]) {
            dp[l] = arr[i];
            
            // Store that this element came after the element at p[l-1]
            if (l > 0) {
                prev[i] = p[l-1];
            }

            p[l] = i;
        }
    }

    // Find length of longest increasing subsequence
    // Time: O(n)
    int ans = 0;
    for (int i = 0; i <= n; i++) {
        if (dp[i] < oo)
            ans = i;
    }

    // Reconstruct the sequence by following the chain of predecessors
    // Time: O(n)
    vector<int> seq(ans);
    int idx = p[ans];
    for (int i = ans - 1; i >= 0; i--) {
        seq[i] = idx;
        idx = prev[idx];
    }

    return seq;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    while (cin >> N) {

        vector<int> arr(N);
        for (int i = 0; i < N; i++) {
            cin >> arr[i];
        }

        vector<int> seq = lis(arr);
        cout << seq.size() << '\n';
        for (int i = 0; i < seq.size(); i++) {
            cout << seq[i] << ' ';
        }
        cout << '\n';
   }

    cout << flush;
    return 0;
}