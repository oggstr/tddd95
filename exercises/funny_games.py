"""
@author Oskar Arensmeier
@date 2024-02-10
"""

import sys

n = int(sys.stdin.readline().strip())

for _ in range(n):

    problem = sys.stdin.readline().strip().split()

    start_state = float(problem[0])
    k           = int(problem[1])
    transitions = list(map(float, problem[2:]))

    memo = {}
    def min_max_search(state: float, is_max: bool, alpha: int, beta: int) -> int:
        if (state, is_max) in memo:
            return memo[(state, is_max)]

        if state <= 1:
            return 1 if is_max else -1

        if is_max:
            v = -float('inf')
            for t in transitions:
                v = max(v, min_max_search(state * t, not is_max, alpha, beta))
                alpha = max(alpha, v)
                if beta <= alpha:
                    break
            memo[(state, is_max)] = v
            return v
        else:
            v = float('inf')
            for t in transitions:
                v = min(v, min_max_search(state * t, not is_max, alpha, beta))
                beta = min(beta, v)
                if beta <= alpha:
                    break
            memo[(state, is_max)] = v
            return v

    result = min_max_search(start_state, True, -float('inf'), float('inf'))
    if result == 1:
        print("Mikael")
    else:
        print("Nils")