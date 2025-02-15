"""
@author Oskar Arensmeier
@date 2024-02-14
"""

import sys

n = int(sys.stdin.readline().strip())

for _ in range(n):

    problem = sys.stdin.readline().strip().split()

    X = float(problem[0])
    K = int(problem[1])
    F = list(map(lambda x : 1/x if x > 0.0 else 99999, map(float, problem[2:]))) # 1 / f    

    F.sort()

    ranges = [(1, F[-1])]
    while not (ranges[-1][0] < X <= ranges[-1][1]):
        ranges.append(
            (
                ranges[-1][1],
                ranges[-1][1] * F[0]
            )
        )

    if len(ranges) % 2 == 1:
        print("Nils")
    else:
        print("Mikael")
