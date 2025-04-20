"""
@author Oskar Arensmeier
@date 2025-04-01

# Algorithm

Dynamic programming using a tabulation approach to solve 0/1 knapsack problem.
Each row represents an item and each column a capacity.
At each cell we simply decide whether to include the current item or not.
This is done by taking the max of the above cell (not including the item)
and the cell by taking minus the current item weight (including the item).

In order to figure out which items were included a backtrace table is used.
It simply stores whether the current item was included or not.
By starting from the bottom right cell we can backtrace to find all indices.

# Datastructures
- 2D list for dp table
- 2D list for backtrace table

# Complexity

The nested loops is O(n * m) where n is the number of items and m is the capacity.
Each operation inside the loops is O(1). Overall complexity is O(n * m).
"""

import sys

def knapsack(capacity: int, items: list[tuple[int, int]]) -> list[int]:
    """
    Solve the 0/1 knapsack problem and returns indices of items used in the solution.

    Args:
        capacity (int): Knapsack capacity
        items (list[tuple[int, int]]): List of items with value and weight

    Returns:
        list[int]: List of indices of items used in solution
    """

    # items x capacity
    dp = [[0] * (capacity + 1) for _ in range(len(items) + 1)]

    # backtrace stores if items were included
    backtrace = [[False] * (capacity + 1) for _ in range(len(items) + 1)]

    for i in range(1, len(items)+1):
        value, weight = items[i-1]

        for c in range(capacity+1):

            if (k := c - weight) >= 0:
                # Decide whether it's best to
                # include or not to include current item
                
                if dp[i-1][k] + value > dp[i-1][c]:
                    dp[i][c]        = dp[i-1][k] + value
                    backtrace[i][c] = True
                else:
                    dp[i][c] = dp[i-1][c]

            else:
                # current item doesn't fit with current capacity,
                # just save value without adding current item
                dp[i][c] = dp[i-1][c]

    item = len(items)
    cap = capacity

    indices = []
    while item > 0:

        # Check if this item was included
        if backtrace[item][cap]:

            indices.append(item-1)
            # offset with item weight
            cap -= items[item-1][1]

        item -= 1

    indices.reverse()
    return indices

if __name__ == "__main__":
    for line in sys.stdin:
        capacity, n = map(int, line.split())
        items = [tuple(map(int, sys.stdin.readline().split(' '))) for _ in range(n)]

        indices = knapsack(capacity, items)

        print(len(indices))
        print(" ".join(map(str, indices)))