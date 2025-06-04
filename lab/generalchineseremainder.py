from typing import NamedTuple
import sys

"""
@author Oskar Arensmeier
@date 2025-05-26
"""

"""
Solves a system of TWO linear congruences
using a general solution of Chinese Remainder Theorem.

Reference: https://en.wikipedia.org/wiki/Chinese_remainder_theorem

Theory:
As the assignment requires only a solution for two linear congruences,
we implement a direct solution given by (derived through Bézout's identity):

x = (avn + bum) / g

where
- x ≡ a (mod n)
- x ≡ b (mod m)
- g = um + vn = gcd(m, n) (Using the extended euclidean from previous assignment)

In addition we may compute least common multiple (lcm) of n and m as:
lcm = (n / g) * m

Lastly, a solution only exists if
a ≡ b (mod g) (or a % g == b % g)

Algorithm:
1. Compute g, u, v = gcd(m, n)
2. Check that solution exists: a % g == b % g
3. Compute lcm = (n / g) * m
4. Compute solution: sol = (a * v * n + b * u * m) / g
5. Normalize solution to range [0, lcm]: sol %= lcm


Time Complexity:
- O(log(min(a, b))) for gcd computation
- O(1) for the CRT solution (as we only have two equations)

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

def crt(a: int, m: int, b: int, n: int) -> tuple[int, int]:
    """
    Solves a system of two linear congruences
    using a general solution of Chinese Remainder Theorem.

    Args:
        a (int): First congruence val
        m (int): First modulus
        b (int): Second congruence val
        n (int): Second modulus

    Returns:
        tuple[int, int]: (solution, lcm), or (-1, -1) if no solution exists.
    """
    # General solution is given by:
    #
    # x = (a*v*n + b*u*m) / g
    #
    # where g = u*m + v*n = gcd(n, m)
    #
    # Source: https://en.wikipedia.org/wiki/Chinese_remainder_theorem#Generalization_to_non-coprime_moduli
    g, u, v = gcd(m, n)

    if a % g != b % g:
        return -1, -1

    lcm = m*n // g 
    sol = (a*v*n + b*u*m) // g
    
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