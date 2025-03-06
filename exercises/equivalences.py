
import sys

def solve(n: int, m: int):
    in_deg  = {}
    out_deg = {}

    for _ in range(m):
        a, b = map(int, sys.stdin.readline().strip().split())

        if b not in in_deg:
            in_deg[b] = 0
        if a not in out_deg:
            out_deg[a] = 0

        in_deg[b]  += 1
        out_deg[a] += 1

    missing_out = sum(1 for i in range(1, n+1) if i not in out_deg)
    missing_in  = sum(1 for i in range(1, n+1) if i not in in_deg)
    missing = max(missing_out, missing_in)

    return missing


if __name__ == "__main__":
    N = int(sys.stdin.readline().strip())
    for _ in range(N):
        n, m = map(int, sys.stdin.readline().strip().split())
        print(solve(n, m))