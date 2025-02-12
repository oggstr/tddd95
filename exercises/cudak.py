"""
@author: Oskar Arensmeier
@date: 2024-02-09
"""
import sys
from functools import lru_cache


def solve(max_num: int, target_sum: int) -> int:
    limits = list(map(int, str(max_num)))
    digits = len(limits)

    @lru_cache(None)
    def dp(digit: int, curr_sum: int, limit: bool) -> int:
        if digit == digits:
            return 1 if curr_sum == target_sum else 0

        limit_digits = limits[digit] if limit else 9
        count = 0
        for i in range(limit_digits + 1):
            if curr_sum + i > target_sum:
                break
            count += dp(digit + 1, curr_sum + i, limit and i == limit_digits)

        return count

    return dp(0, 0, True)

def solve_interval(A: int, B: int, S: int) -> int:
    return solve(B, S) - solve(A - 1, S)

data = sys.stdin.read().strip().split()
A, B, S = map(int, data)

print(solve_interval(A, B, S))

# Bin search
low, high = A, B
while low < high:
    mid = (low + high) // 2
    if solve_interval(low, mid, S) > 0:
        high = mid
    else:
        low = mid + 1

print(low)