"""
@author Oskar Arensmeier
@date 2024-02-16
"""

import sys
import heapq

class State:

    # X, Y
    pos: tuple[int, int]

    # steps / time
    steps: int
    
    cost: int

    def __init__(
        self: "State",
        pos: tuple[int, int],
        steps: int,
        cost: int
    ) -> None:
        self.pos = pos
        self.steps = steps
        self.cost = cost

    def __lt__(self: "State", other: "State") -> bool:
        return self.cost < other.cost

    def __eq__(self: "State", other: "State") -> bool:
        return self.pos == other.pos and self.steps == other.steps

    def __hash__(self: "State") -> int:
        return hash((self.pos, self.steps))

    def is_final(self: "State", goal: tuple[int, int]) -> bool:
        return self.pos == goal

def manhattan_distance(a: tuple[int, int], b: tuple[int, int]) -> int:
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

def main():
    MAX_STEPS = int(sys.stdin.readline().strip())

    # number lanes, length lanes
    n, m = map(int, sys.stdin.readline().strip().split(" "))

    lanes = []
    for _ in range(n+2):
        lanes.append(list(sys.stdin.readline().strip()))
    lanes.reverse()

    start = (lanes[0].index("F"), 0)
    goal  = (lanes[-1].index("G"), len(lanes) - 1)

    # Heuristic function
    h = manhattan_distance

    visited = set()

    pq = [State(start, 0, h(start, goal))]
    solution = None
    while pq:
        state = heapq.heappop(pq)

        if state in visited:
            continue

        visited.add(state)

        if state.is_final(goal):
            solution = state
            break

        x, y = state.pos
        for dx, dy in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
            nx, ny = x + dx, y + dy

            if nx < 0 or nx >= m or ny < 0 or ny >= len(lanes):
                continue

            # Avoid obstacles
            offset = (state.steps + 1) % m
            if ny % 2 == 1 and lanes[ny][nx - offset] == "X":
                continue
            elif ny % 2 == 0 and lanes[ny][(nx + offset) % m] == "X":
                continue

            next_state = State(
                (nx, ny),
                state.steps + 1,
                state.steps + 1 + h((nx, ny), goal)
            )

            if next_state.steps > MAX_STEPS:
                continue

            heapq.heappush(
                pq,
                next_state
            )

    if solution:
        print(f"The minimum number of turns is {solution.steps}.")
    else:
        print("The problem has no solution.")

if __name__ == "__main__":
    for _ in range(int(sys.stdin.readline().strip())):
        main()