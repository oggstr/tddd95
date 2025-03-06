"""
@author Oskar Arensmeier
@date 2024-02-20
"""

"""
@Note: Doesn't work. A single test case fails - off by one.
I suspect this method can form a cycle somehow.
"""

import sys
from collections import deque
from typing import Tuple, List

def find(parent: dict, x: Tuple[int, int]) -> Tuple[int, int]:
    if parent[x] != x:
        parent[x] = find(parent, parent[x])
    return parent[x]

def union(parent: dict, a: Tuple[int, int], b: Tuple[int, int]):
    ra = find(parent, a)
    rb = find(parent, b)

    parent[ra] = rb

class Cell:
    def __init__(self, wall: bool):
        self.wall: bool = wall

        # (origin, steps taken from origin)
        self.visited: Tuple[Tuple[int, int], int] | None = None

    def is_wall(self) -> bool:
        return self.wall

    def mark(self, source: Tuple[int, int], steps: int) -> None:
        self.visited = (source, steps)

def read_board() -> Tuple[int, int, List[List[str]]]:
    X, Y = map(int, sys.stdin.readline().strip().split())

    board: List[List[str]] = []
    for _ in range(Y):
        line = sys.stdin.readline().rstrip("\n")

        # Pad with walls if needed
        if len(line) < X:
            line += "#" * (X - len(line))

        board.append(list(line))

    return X, Y, board

def solve() -> int:
    X, Y, board = read_board()

    search_grid: List[List[Cell]] = [[Cell(board[y][x] == "#") for x in range(X)] for y in range(Y)]

    starters: List[Tuple[int, int]] = []
    for y in range(Y):
        for x in range(X):
            if board[y][x] in ["S", "A"]:
                starters.append((x, y))

    # union-find set
    parent = {src: src for src in starters}
    num_components = len(starters)

    frontier = deque()
    for src in starters:
        x, y = src
        search_grid[y][x].mark(src, 0)
        frontier.append((x, y, 0, src))

    result = 0
    moves = [(0,1),(1,0),(0,-1),(-1,0)]

    while num_components > 1:

        x, y, steps, origin = frontier.popleft()

        for dx, dy in moves:
            nx, ny = x + dx, y + dy

            if not (0 <= nx < X and 0 <= ny < Y):
                continue

            if search_grid[ny][nx].is_wall():
                continue

            # two frontiers meet
            if search_grid[ny][nx].visited is not None:

                other_origin, other_steps = search_grid[ny][nx].visited

                # frontiers have different origins and belong to different components
                if other_origin != origin and find(parent, origin) != find(parent, other_origin):

                    # steps from current origin + 1 (for move) + steps from other origin
                    result += steps + 1 + other_steps
                    union(parent, origin, other_origin)
                    num_components -= 1

            else:
                search_grid[ny][nx].mark(origin, steps + 1)
                frontier.append((nx, ny, steps + 1, origin))

    return result

def main() -> None:
    N = int(sys.stdin.readline().strip())

    for _ in range(N):
        result = solve()
        print(result)

if __name__ == "__main__":
    main()