/**
 * @author Oskar Arensmeier
 * @date 2025-04-01
 *
 * # Algorithm
 *
 * Dynamic programming using a tabulation approach to solve 0/1 knapsack problem.
 * Each row represents an item and each column a capacity.
 * At each cell we simply decide whether to include the current item or not.
 * This is done by taking the max of the above cell (not including the item)
 * and the cell by taking minus the current item weight (including the item).
 *
 * In order to figure out which items were included a backtrace table is used.
 * It simply stores whether the current item was included or not.
 * By starting from the bottom right cell we can backtrace to find all indices.
 *
 * # Datastructures
 * - 2D list for dp table
 * - 2D list for backtrace table
 *
 * # Complexity
 *
 * The nested loops is O(n * m) where n is the number of items and m is the capacity.
 * Each operation inside the loops is O(1). Overall complexity is O(n * m).
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Solve the 0/1 knapsack problem
 * and returns indices of items used in the solution.
 * 
 * @param capacity Knapsack capacity
 * @param items List of items with value and weight
 * @return List of indices of items used in solution
 */
vector<int> cover(
    int capacity,
    const vector<pair<int, int>>& items
) {
    int n = items.size();

    // items x capacity
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    // backtrace stores if items were included
    vector<vector<bool>> backtrace(n + 1, vector<bool>(capacity + 1, false));

    for (int i = 1; i <= n; ++ i) {
        int value = items[i-1].first;
        int weight = items[i-1].second;

        for (int c = 0; c <= capacity; ++ c) {

            if (int k = c - weight; k >= 0) {
                // Decide whether it's best to
                // include or not to include current item
                if (dp[i-1][k] + value > dp[i-1][c]) {
                    dp[i][c] = dp[i-1][k] + value;
                    backtrace[i][c] = true;
                } else {
                    dp[i][c] = dp[i-1][c];
                }
            } else {
                // Current item doesn't fit with current capacity,
                // just save value without adding current item
                dp[i][c] = dp[i-1][c];
            }
        }
    }

    // Backtrace to find indices of items used in solution
    vector<int> indices;
    int curr_capacity = capacity;
    for (int i = n; i > 0; --i) {
        if (backtrace[i][curr_capacity]) {
            indices.push_back(i-1);
            curr_capacity -= items[i-1].second;
        }
    }

    reverse(indices.begin(), indices.end());
    return indices;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);


    int capacity, n;
    while (cin >> capacity >> n)
    {
        vector<pair<int, int>> items(n);
        for (int i = 0; i < n; ++i) {
            cin >> items[i].first >> items[i].second;
        }

        vector<int> result = cover(capacity, items);
        cout << result.size() << '\n';
        for (int i = 0; i < result.size(); ++i) {
            cout << result[i] << ' ';
        }
        cout << '\n';
    }

    cout << flush;
}