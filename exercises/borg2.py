"""
@author Oskar Arensmeier
@date 2024-02-21
"""

"""
@NOTE: Works, but is too slow.
"""

import sys
from collections import deque

def read_board() -> tuple[int, int, list[list[str]]]:
    X, Y = map(int, sys.stdin.readline().strip().split())

    board: list[list[str]] = []
    for _ in range(Y):
        line = sys.stdin.readline().rstrip("\n")

        # Pad with walls if needed
        if len(line) < X:
            line += "#" * (X - len(line))

        board.append(list(line))

    return X, Y, board

def bfs(
    source: tuple[int, int],
    targets: list[tuple[int, int]],
    board: list[list[str]],
    graph: dict[tuple[int, int], list[tuple[int, int]]]
) -> None:
    visited = set()
    queue = deque()

    visited.add(source)
    queue.append((source, 0))

    while queue:
        current, steps = queue.popleft()

        if current in targets:
            graph[source].append((current, steps))
            continue

        for dx, dy in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
            x, y = current
            x += dx
            y += dy

            if x < 0 or x >= len(board[0]) or y < 0 or y >= len(board):
                continue

            if board[y][x] in "#":
                continue

            if (x, y) in visited:
                continue

            visited.add((x, y))
            queue.append(((x, y), steps + 1))

def find(parent: dict, x: tuple[int, int]) -> tuple[int, int]:
    if parent[x] != x:
        parent[x] = find(parent, parent[x])
    return parent[x]

def union(parent: dict, x: tuple[int, int], y: tuple[int, int]) -> bool:
    x = find(parent, x)
    y = find(parent, y)

    if x != y:
        parent[x] = y
        return True

    return False

def solve() -> None:
    X, Y, board = read_board()

    graph = {}
    for y in range(Y):
        for x in range(X):
            if board[y][x] in "# ":
                continue

            graph[(x, y)] = []

    # BFS from every start node
    starts = graph.keys()
    for start in starts:
        bfs(start, [p for p in starts if p != start], board, graph)

    edges = [(origin, weight, dest) for origin, neighbors in graph.items() for dest, weight in neighbors]

    # Kruskal's algorithm
    edges.sort(key=lambda x: x[1])
    parent = {start: start for start in starts}

    result = 0
    for origin, weight, dest in edges:
        if union(parent, origin, dest):
            result += weight

    return result

if __name__ == "__main__":
    N = int(sys.stdin.readline().strip())
    for _ in range(N):
        s = solve()
        print(s)