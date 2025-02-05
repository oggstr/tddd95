"""
@author: Oskar Arensmeier
@date: 2025-02-01
"""

import sys
from copy import deepcopy

def solve() -> list[int]:
    N = int(sys.stdin.readline().strip())

    stack: list[set] = []
    result = []
    for _ in range(N):
        cmd = sys.stdin.readline().strip()

        match cmd:
            case "PUSH":
                stack.append(set())
            case "DUP":
                stack.append(deepcopy(stack[-1]))
            case "UNION":
                stack[-2] = stack[-2].union(stack[-1])
                stack.pop()
            case "INTERSECT":
                stack[-2] = stack[-2].intersection(stack[-1])
                stack.pop()
            case "ADD":
                item = list(stack[-1])
                item.sort()
                item = "(" + "".join(item) + ")"

                stack[-2].add(item)
                stack.pop()

            case _:
                print("Error")
                return []

        result.append(len(stack[-1]))

    return result


if __name__ == '__main__':
    T = int(sys.stdin.readline().strip())
    for _ in range(T):
        out = solve()
        for o in out:
            print(o)
        print("***")