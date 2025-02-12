"""
@author: Oskar Arensmeier
@date: 2024-02-09
"""

from typing import Callable
from functools import lru_cache
import sys

OUTPUT = ["NNN", "NNY", "NYN", "NYY", "YNN", "YNY", "YYN", "YYY"]

n = int(sys.stdin.readline().strip())

for _ in range(n):
    m = int(sys.stdin.readline().strip())

    priorities = [list(map(int, sys.stdin.readline().strip().split())) for _ in range(m)]

    # Initialize outcomes as being themselves
    outcome = list(range(8))

    # Iterate over priorities in reverse order
    for priority in reversed(priorities):

        # Have every voter minimize the outcome by
        # looking at the outcomes # of later voters
        outcome = [
            min(
                [outcome[state ^ flip] for flip in [0b001, 0b010, 0b100]],
                key = lambda x: priority[x])
            for state in range(0, 8)]

    print(OUTPUT[outcome[0]])
