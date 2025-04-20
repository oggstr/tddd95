"""
@author Oskar Arensmeier
@date 2025-04-20
"""

import sys

def is_match(suffix: str, original: str, pos: int) -> bool:
    for i in range(len(suffix)):
        if original[pos + i] != "-" and original[pos + i] != suffix[i]:
            return False
    return True

def reconstruct(l: int, s: int, suffixes: list[tuple[int, str]]) -> str:
    original = "-" * l
    for suffix in suffixes:
        pos, suffix = suffix

        # Check for "*"
        if suffix.find("*") != -1:
            suffix, end = suffix.split("*")

            # Check that end of pattern matches
            if not is_match(end, original, l-len(end)):
                return "Impossible"

            original = original[:l-len(end)] + end

        if not is_match(suffix, original, pos):
            return "Impossible"

        original = original[:pos] + suffix + original[pos+len(suffix):]

    if "-" in original:
        return "Impossible"

    return original

if __name__ == "__main__":
    N = int(sys.stdin.readline().strip())

    for _ in range(N):
        l, s = map(int, sys.stdin.readline().strip().split())
        
        suffixes = []
        for _ in range(s):
            pos, suffix = sys.stdin.readline().strip().split()
            # Correct for 1-indexing
            suffixes.append((int(pos)-1, suffix))

        #suffixes.sort(key=lambda x: x[0])
        print(reconstruct(l, s, suffixes))