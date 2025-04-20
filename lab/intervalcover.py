"""
@author Oskar Arensmeier
@date 2025-04-01

# Algorithm

1. Sort segments by start point
2. Init current end point to a and index to 0.
3. While current end point is less than b:
    1. Find segment with the maximum end point that starts before or at the current end point
    2. If no such segment exists, return None
    3. Update current end point to the end point of the selected segment, and add its index to the result list
4. Return the number of segments used and their indices.

# Complexity

There are two main operations
- Sorting the segments in O(n log n) (Exact method depends on the python implementation used, but it's O(n log n) in regardless)
- Iterating through the segments in O(n)

Since these operations are done in sequence, the overall complexity is O(n log n)
"""

import sys

def cover(
    interval: tuple[float, float],
    segments: list[tuple[float, float]]
) -> list[int, list[int]] | None:
    """
    Solve the interval cover problem.
    Complexity: O(n log n)

    Args:
        a (float): Start of interval
        b (float): End of interval
        segments (list[tuple[float, float]]): List of segments

    Returns:
        list[int, list[int]] | None:
            List of two elements:
                - Number of segments used
                - List of indices of segments used
            None: If no segments can cover the interval
    """

    a, b = interval

    # Also store original index of segment to avoid expensive lookup
    segments_sorted = [(s[0], s[1], i) for s, i in zip(segments, range(len(segments)))]

    # O(n log n)
    segments_sorted.sort(key=lambda s: s[0])

    curr_end = a
    idx      = 0
    count    = 0

    result_idx = []

    # Using do-while loop clear edge case where interval is zero length
    # O(n)
    while True:

        best_seg = None

        while idx < len(segments) and segments_sorted[idx][0] <= curr_end:

            if best_seg is None or best_seg[1] < segments_sorted[idx][1]:
                best_seg = segments_sorted[idx]

            idx += 1

        if best_seg is None:
            return None

        curr_end = best_seg[1]

        result_idx.append(best_seg[2])
        count += 1

        if curr_end >= b:
            break

    return [count, result_idx]

if __name__ == "__main__":
    for line in sys.stdin:
        a, b = map(float, line.split())

        n = int(sys.stdin.readline())
        segments = [tuple(map(float, sys.stdin.readline().split())) for _ in range(n)]
        segments = list(segments)

        result = cover((a, b), segments)
        if result is None:
            print("impossible")
        else:
            print(result[0])
            print(" ".join(map(str, result[1])))