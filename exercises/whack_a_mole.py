"""
@author Oskar Arensmeier
@version 2024-02-10
"""
import sys
from math import gcd

def moles_on_path(
    moles: list[list[int]],
    p1: tuple[int, int],
    p2: tuple[int, int]
) -> int:

    x1, y1 = p1
    x2, y2 = p2

    dx = x2 - x1
    dy = y2 - y1

    if dx == 0 and dy == 0:
        return moles[x1][y1]

    divisor = gcd(dx, dy)
    dx //= divisor
    dy //= divisor

    # Count starting position
    mole_count = moles[x1][y1]

    # Count any moles on the path
    while x1 != x2 or y1 != y2:
        x1 += dx
        y1 += dy

        #if x1 < 0 or x1 >= len(moles) or y1 < 0 or y1 >= len(moles):
        #    break

        mole_count += moles[x1][y1]

    return mole_count


while True:
    n, d, m = map(int, sys.stdin.readline().strip().split())

    # Allow hammer to move outside the mole area
    # This allows "lines" that would otherwise not be possible
    n += 2*d

    if n == 0 and d == 0 and m == 0:
        break

    moles = [[[0 for _ in range(n)] for _ in range(n)] for _ in range(10)]
    for i in range(m):
        x, y, t = map(int, sys.stdin.readline().strip().split())

        # Offset moles with the padding added to n
        moles[t-1][d+x][d+y] = 1

    # For each time step, grid sized n x n
    dp = [[[0 for _ in range(n)] for _ in range(n)] for _ in range(10)]

    # Init first time step
    # For every starting pos
    for x in range(0, n):
        for y in range(0, n):

            # For every reachable spot
            for i in range(-d, d+1):
                for j in range(-d, d+1):

                    if x+i < 0 or x+i >= n or y+j < 0 or y+j >= n:
                        continue

                    if i**2 + j**2 <= d**2:

                        dp[0][x+i][y+j] = max(
                            dp[0][x+i][y+j],
                            moles_on_path(moles[0], (x, y), (x+i, y+j))
                        )


    # At every time step we
    # 1. We go from every starting position
    # 2. To every reachable spot
    # 3. At the reached spot, we calculate the max amount of moles we can hit
    for t in range(1, 10):
        for x in range(0, n):
            for y in range(0, n):
                for i in range(-d, d+1):
                    for j in range(-d, d+1):

                        if x+i < 0 or x+i >= n or y+j < 0 or y+j >= n:
                            continue

                        if i**2 + j**2 <= d**2:
                            dp[t][x+i][y+j] = max(
                                dp[t][x+i][y+j],
                                dp[t-1][x][y] + moles_on_path(moles[t], (x, y), (x+i, y+j))
                            )

    best = 0
    for x in range(0, n):
        for y in range(0, n):
            best = max(best, dp[9][x][y])

    print(best)
