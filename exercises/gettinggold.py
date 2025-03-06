"""
@author Oskar Arensmeier
@date 2024-02-16
"""

import sys

COLS, ROWS = map(int, sys.stdin.readline().strip().split(" "))

board = [list(sys.stdin.readline().strip()) for _ in range(ROWS)]
reached = [[False for _ in range(COLS)] for _ in range(ROWS)]

def find_start_pos() -> tuple[int, int]:
    global board
    for x in range(ROWS):
        for y in range(COLS):
            if board[x][y] == "P":
                return (x, y)

def next_positions(pos: tuple[int, int]) -> list[tuple[int, int]]:
    global board
    x, y = pos

    positions = []

    if board[x+1][y] != "#":
        positions.append((x+1, y))
    if board[x-1][y] != "#":
        positions.append((x-1, y))
    if board[x][y+1] != "#":
        positions.append((x, y+1))
    if board[x][y-1] != "#":
        positions.append((x, y-1))

    return positions

start = find_start_pos()
reached[start[0]][start[1]] = True
stack = [start]

while stack:
    pos = stack.pop()

    neighbors = next_positions(pos)

    # We cannot safely move from this position
    # as there are traps nearby
    if any(map(lambda p: board[p[0]][p[1]] == "T", neighbors)):
        continue

    for n in neighbors:
        if reached[n[0]][n[1]]:
            continue

        reached[n[0]][n[1]] = True
        stack.append(n)


gold = 0
for x in range(ROWS):
    for y in range(COLS):
        if reached[x][y] and board[x][y] == "G":
            gold += 1

print(gold)