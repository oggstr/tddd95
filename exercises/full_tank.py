"""
@author Oskar Arensmeier
@date 2024-02-26
"""
import sys

from typing import NamedTuple
from collections import defaultdict

Edge  = NamedTuple("Edge", [("u", int), ("v", int), ("cost", int)])
Query = NamedTuple("Query", [("cap", int), ("start", int), ("end", int)])

def parse_input() -> tuple[int, int, list[int], list[Edge], int, list[Query]]:
    N, M = map(int, sys.stdin.readline().strip().split())

    cities = list(map(int, sys.stdin.readline().strip().split()))
    edges  = []
    for _ in range(M):
        edges.append(Edge(*map(int, sys.stdin.readline().strip().split())))

    q = int(sys.stdin.readline().strip())
    queries = []
    for _ in range(q):
        queries.append(Query(*map(int, sys.stdin.readline().strip().split())))

    return N, M, cities, edges, q, queries

def dfs(
    graph: dict[int, list[int]],
    cities: list[int],
    dists: dict[tuple[int, int], int],
    curr: int,
    end: int,
    cap: int,
    cost: int,
    fuel: int,
    visited: list[bool],
) -> list[int]:
    if curr == end:
        return [cost]

    candidate_solutions = []
    visited[curr] = True
    for f in range(fuel, cap+1):
        for n in graph[curr]:

            if visited[n]:
                continue

            if f < dists[(curr, n)]:
                continue

            candidate_solutions.extend(
                dfs(
                    graph,
                    cities,
                    dists,
                    n,
                    end,
                    cap,
                    cost + cities[curr]*(f - fuel),
                    f - dists[(curr, n)],
                    visited
                )
            )

    visited[curr] = False
    return candidate_solutions

def main() -> int | str:
    N, _, cities, edges, _, queries = parse_input()

    graph = defaultdict(list)
    dists = {}
    for e in edges:
        graph[e.u].append(e.v)
        graph[e.v].append(e.u)
        dists[(e.u, e.v)] = e.cost
        dists[(e.v, e.u)] = e.cost

    solutions = []
    for q in queries:
        candidate_solutions = dfs(
            graph,
            cities,
            dists,
            q.start,
            q.end,
            q.cap,
            0,
            0,
            [False] * (N+1)
        )

        best = float("inf")
        for sol in candidate_solutions:
            best = min(best, sol)

        solutions.append(best)

    return solutions

if __name__ == "__main__":
    for s in main():
        if s == float("inf"):
            print("impossible")
        else:
            print(s)