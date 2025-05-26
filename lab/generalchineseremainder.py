from typing import NamedTuple
import sys

"""
@author Oskar Arensmeier
@date 2025-05-26
"""

"""
Solves a system of two linear congruences
using a general solution of Chinese Remainder Theorem.

Reference: https://en.wikipedia.org/wiki/Chinese_remainder_theorem

Algorithm:
Implements a direct solution using Bézout's identity.

Time Complexity:
- O(log(min(a, b))) for gcd computation
- O(1) for the CRT solution

Space Complexity:
- O(1)

Usage:
Only supports a system of two linear congruences.
Does NOT require moduli to be coprime!
"""

def gcd(a: int, b: int) -> tuple[int, int, int]:
    """ Compute gcd using Extended Euclidean Algorithm.
    Time: O(log(min(a, b)))

    Args:
        a (int): First number
        b (int): Second number

    Returns:
        tuple[int, int, int]: divisor, x, y such that a*x + b*y = gcd(a, b)
    """
    if b == 0:
        return a, 1, 0

    d, x1, y1 = gcd(b, a % b)

    x = y1
    y = x1 - y1 * (a // b)

    return d, x, y

def crt(a: int, n: int, b: int, m: int) -> tuple[int, int]:
    """
    Solves a system of two linear congruences
    using a general solution of Chinese Remainder Theorem.

    Args:
        a (int): First congruence val
        n (int): First modulus
        b (int): Second congruence val
        m (int): Second modulus

    Returns:
        tuple[int, int]: (solution, lcm), or (-1, -1) if no solution exists.
    """
    # General solution is given by:
    # 
    # z = (a * m * y + b * n * x) / g
    # 
    # where n * x + m * y = g = gcd(n, m)
    # 
    # Source: https://en.wikipedia.org/wiki/Chinese_remainder_theorem#Generalization_to_non-coprime_moduli
    g, x, y = gcd(n, m)
    
    if a % g != b % g:
        return -1, -1

    lcm = (n // g) * m 
    sol = (a * m * y + b * n * x) // g
    
    # Normalize solution to range [0, lcm]
    sol %= lcm
    return sol, lcm

def main():
    T = int(sys.stdin.readline().strip())
    for _ in range(T):

        a, n, b, m = map(int, sys.stdin.readline().strip().split())
        sol, lcm = crt(a, n, b, m)

        if sol == -1:
            print("no solution")
        else:
            print(f"{sol} {lcm}")

if __name__ == "__main__":
    main()