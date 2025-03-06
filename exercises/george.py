"""
@author Oskar Arensmeier
@date 2024-02-23
"""
import sys
import heapq
from collections import defaultdict

def main():
    _, _, start, goal, K, _, george, graph, costs = parse_input()
    print(dijkstra(graph, costs, start, goal, K, george))

def parse_input() -> tuple[int, int, int, int, int, int, list[tuple[int, int]], dict[int, list[int]], dict[tuple[int, int], int]]:
    N, M = map(int, sys.stdin.readline().strip().split())
    A, B, K, G = map(int, sys.stdin.readline().strip().split())

    george = list(map(int, sys.stdin.readline().strip().split()))
    george: list[tuple[int, int]] = list(zip(george[:-1], george[1:]))

    graph = defaultdict(list)
    costs = {}
    for _ in range(M):
        a, b, c = map(int, sys.stdin.readline().strip().split())

        graph[a].append(b)
        graph[b].append(a)

        costs[(a, b)] = c
        costs[(b, a)] = c

    return N, M, A, B, K, G, george, graph, costs

def dijkstra(
    graph: dict[int, list[int]],
    costs: dict[int, list[int]],
    start_node: int,
    goal_node: int,
    start_time: int,
    george: list[tuple[int, int]]
) -> int:
    street_blocked = {}
    time = 0
    for a, b in george:
        time_blocked = costs[(a, b)]
        street_blocked[(a, b)] = (time, time + time_blocked)
        street_blocked[(b, a)] = (time, time + time_blocked)
        time = time + time_blocked

    dist = {node: float("inf") for node in graph}
    dist[start_node] = start_time

    q = [(start_time, start_node)]
    heapq.heapify(q)

    while q:
        curr_time, curr_node  = heapq.heappop(q)

        if curr_node == goal_node:
            return curr_time - start_time

        if curr_time > dist[curr_node]:
            continue

        for neighbor in graph[curr_node]:

            next_time = curr_time + costs[(curr_node, neighbor)]
            if (curr_node, neighbor) in street_blocked:

                block_start, block_end = street_blocked[(curr_node, neighbor)]
                if block_start <= curr_time < block_end:
                    next_time = block_end + costs[(curr_node, neighbor)]


            if next_time < dist[neighbor]:
                dist[neighbor] = next_time
                heapq.heappush(q, (next_time, neighbor))

if __name__ == "__main__":
    main()