"""
@author Oskar Arensmeier
@date 2024-02-11
"""

import sys

n = int(sys.stdin.readline().strip())

for _ in range(n):

    problem = sys.stdin.readline().strip().split()

    start_state = float(problem[0])
    k           = int(problem[1])
    transitions = list(map(float, problem[2:]))

    max_val = max(transitions)
    min_val = min(transitions)

    i = 0
    min_range = 1.0
    max_range = 1.0
    while not (min_range <= start_state <= max_range):
        i += 1
        min_range = min_range / max_val
        max_range = max_range / min_val

    print(i)
    if i % 2 == 1:
        print("Nils")
    else:
        print("Mikael")
