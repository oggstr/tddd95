"""
@author Oskar Arensmeier
@date 2024-02-27
"""
import sys

from copy import deepcopy

adj_list = list[list[int]]

def push(
    flow: adj_list,
    capacity: adj_list,
    excess: list[int],
    u: int,
    v: int,
) -> None:
    d = min(excess[u], capacity[u][v] - flow[u][v])
    flow[u][v] += d
    flow[v][u] -= d
    excess[u] -= d
    excess[v] += d

def relabel(
    N: int,
    flow: adj_list,
    capacity: adj_list,
    excess: list[int],
    height: list[int],
    u: int
) -> None:
    d = float("inf")

    for v in range(N):
        if capacity[u][v] - flow[u][v] > 0:
            d = min(d, height[v])

    if d < float("inf"):
        height[u] = d + 1

def fin_max_height(
    N: int,
    excess: list[int],
    height: list[int],
    s: int,
    t: int
) -> list[int]:
    max_height_vertices = []

    for v in range(N):
        if v != s and v != t and excess[v] > 0:

            if max_height_vertices and height[v] > height[max_height_vertices[0]]:
                max_height_vertices = [] # clear

            if not max_height_vertices or height[v] == height[max_height_vertices[0]]:
                max_height_vertices.append(v)

    return max_height_vertices

def max_flow(
    N: int,
    flow: adj_list,
    capacity: adj_list,
    excess: list[int],
    height: list[int],
    s: int,
    t: int,
):

    for v in range(N):
        if s != v:
            push(flow, capacity, excess, s, v)

    current = []
    while True:
        current = fin_max_height(N, excess, height, s, t)

        if not current:
            break

        for u in current:
            pushed = False

            for v in range(N):

                if not excess[u]:
                    break

                if capacity[u][v] - flow[u][v] > 0 and height[u] == height[v] + 1:
                    push(flow, capacity, excess, u, v)
                    pushed = True

            if not pushed:
                relabel(N, flow, capacity, excess, height, u)

    return excess[t]

def main():
    n = int(sys.stdin.readline().strip())

    names       = list()
    all_parties = set()
    all_clubs   = set()

    lines = []
    for _ in range(n):
        name, party, _, *clubs = sys.stdin.readline().strip().split()
        #print(name, party, clubs)
        lines.append((name, party, clubs))

        names.append(name)
        all_parties.add(party)
        all_clubs.update(clubs)

    # +2 for start and end nodes
    N = len(names) + len(all_parties) + len(all_clubs) + 2

    id_map = dict()
    id_map["start"] = 0
    id_map["end"]   = 1

    for i, name in enumerate(names):
        id_map[name] = i + 2
    for i, party in enumerate(all_parties):
        id_map[party] = i + 2 + len(names)
    for i, club in enumerate(all_clubs):
        id_map[club] = i + 2 + len(names) + len(all_parties)

    # Push-relabel algorithm
    #graph    = [[0 for _ in range(N)] for _ in range(N)]
    flow     = [[0 for _ in range(N)] for _ in range(N)]
    capacity = [[0 for _ in range(N)] for _ in range(N)]
    excess   = [0 for _ in range(N)]
    height   = [0 for _ in range(N)]

    height[id_map["start"]] = N
    excess[id_map["start"]] = N

    party_max_capacity = len(names) // 2 - 1

    for name, party, clubs in lines:
        #graph[id_map["start"]][id_map[party]] = 1
        #graph[id_map[party]][id_map[name]]    = 1

        capacity[id_map["start"]][id_map[party]] = max(0, party_max_capacity)
        capacity[id_map[party]][id_map[name]]    = 1

        for club in clubs:
            #graph[id_map[name]][id_map[club]] = 1
            #graph[id_map[club]][id_map["end"]] = 1

            capacity[id_map[name]][id_map[club]] = 1
            capacity[id_map[club]][id_map["end"]] = 1

    result = max_flow(N, flow, capacity, excess, height, id_map["start"], id_map["end"])

    if result != len(all_clubs):
        print("Impossible.")
        return

    for name, party, clubs in lines:
        for club in clubs:
            if flow[id_map[name]][id_map[club]] == 1:
                print(name, club)

    print()

if __name__ == "__main__":
    T = int(sys.stdin.readline().strip())
    for _ in range(T):
        main()