import sys
from collections import defaultdict
import heapq

def is_possible(word: str) -> bool:
    letter_count = defaultdict(int)
    for l in word:
        letter_count[l] += 1

    uneven = 0
    for l, c in letter_count.items():
        if c % 2 == 1:
            uneven += 1

    return uneven <= 1

def is_palindrome(word: str) -> bool:
    return word == word[::-1]

def bfs(word: str) -> int:
    q = [(0, word)]

    while True:
        s, w = heapq.heappop(q)
        if is_palindrome(w):
            return s

        for i in range(len(w)-1):
            heapq.heappush(q, (s + 1, w[:i] + w[i+1] + w[i] + w[i+2:]))

if __name__ == "__main__":
    N = int(sys.stdin.readline().strip())

    for _ in range(N):
        word = sys.stdin.readline().strip()

        if not is_possible(word):
            print("Impossible")
            continue

        print(bfs(word))