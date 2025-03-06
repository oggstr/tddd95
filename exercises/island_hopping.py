"""
@author Oskar Arensmeier
@date 2024-02-25
"""
import sys

from math import sqrt, pow
from typing import NamedTuple

Point = NamedTuple("Point", [("x", float), ("y", float)])
Edge = NamedTuple("Edge", [("u", int), ("v", int), ("weight", float)])

def find(uf: list[int], i: int) -> int:
    if uf[i] == i:
        return i

    uf[i] = find(uf, uf[i])
    return uf[i]

def union(uf: list[int], i: int, j: int) -> bool:
    i, j = find(uf, i), find(uf, j)
    if i == j:
        return False

    uf[j] = i
    return True

def main() -> float:
    M, points = parse_input()

    edges: list[Edge] = []
    dist = lambda p1, p2: sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2))
    for i in range(M):
        for j in range(i+1, M):
            edges.append(Edge(i, j, dist(points[i], points[j])))

    edges.sort(key = lambda e: e.weight)

    uf = [i for i in range(M)]

    result = 0
    count  = 0
    for e in edges:
        if find(uf, e.u) != find(uf, e.v):
            union(uf, e.u, e.v)
            result += e.weight

            count += 1
            if count == M - 1:
                break

    return result

def parse_input() -> tuple[int, list[Point]]:
    M = int(sys.stdin.readline().strip())
    points: list[Point] = [Point(*map(float, sys.stdin.readline().strip().split())) for _ in range(M)]
    return M, points

if __name__ == "__main__":
    N = int(sys.stdin.readline().strip())
    for _ in range(N):
        print(main())