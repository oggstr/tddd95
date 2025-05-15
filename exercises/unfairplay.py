import sys

def dfs(
    state: list[int],
    matches: list[tuple[int, int]],
    max_score
) -> list[int]:

    if not matches:
        return [-1]

    t1, t2 = matches[0]
    t1, t2 = t1 - 1, t2 - 1
    s1, s2 = state[t1], state[t2]

    if s1 == len(state) - 1:
        state[t1] = s1 + 2
        res = dfs(state, matches[1:], max_score)

        if not res:
            return []

        return [0] + res

    if s2 == len(state) - 1:
        state[t2] = s2 + 2
        res = dfs(state, matches[1:], max_score)

        if not res:
            return []

        return [2] + res

    # t1 wins
    if s1 + 2 <= max_score:
        next_state = state[:]
        next_state[t1] = s1 + 2
        res = dfs(next_state, matches[1:], max_score)
        
        if res:
            return [0] + res


    # t2 wins
    if s2 + 2 <= max_score:
        next_state = state[:]
        next_state[t2] = s2 + 2
        res = dfs(next_state, matches[1:], max_score)

        if res:
            return [2] + res

    # t1 and t2 draw
    if s1 + 1 <= max_score and s2 + 1 <= max_score:
        next_state = state[:]
        next_state[t1] = s1 + 1
        next_state[t2] = s2 + 1
        res = dfs(next_state, matches[1:], max_score)

        if res:
            return [1] + res

    return []

def main():
    while True:
        line = sys.stdin.readline().strip()

        if not line:
            continue

        if line == "-1":
            return

        N, M = map(int, line.split())

        scores = list(map(int, sys.stdin.readline().strip().split()))
        matches = []

        for _ in range(M):
            match = tuple(map(int, sys.stdin.readline().strip().split()))
            matches.append(match)

        max_score = scores[-1] + sum(map(lambda m: m[0] == N-1 or m[1] == N-1, matches)) * 2

        res = dfs(scores, matches, max_score)

        if res:
            print(" ".join(map(str, res[:-1])))
        else:
            print("NO")

if __name__ == "__main__":
    main()