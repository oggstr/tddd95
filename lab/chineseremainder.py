from typing import NamedTuple
import sys

"""
@author Oskar Arensmeier
@date 2025-05-26
"""

"""
Implementation of Chinese Remainder Theorem (CRT)
to solve systems of linear congruences.

reference: https://cp-algorithms.com/algebra/chinese-remainder-theorem.html

Algorithm:
Direct implementation of Chinese Remainder Theorem. 

Time Complexity:
- O(n) amount of iterations for n equations
- O(log(min(a, m))) for gcd computation

Total: O(n * log(min(a, m)))
where n is the number of equations and a and m
are the largest values in the equations (x ≡ a (mod m)).

Space Complexity:
- O(1)

Usage:
Modules terms must be pairwise coprime.
"""

# x ≡ a (mod m)
Congruence = NamedTuple('Congruence', [('a', int), ('m', int)])

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

def mod_inv(a: int, m: int) -> int:
    """Find modular inverse of a mod m

    Args:
        a (int): Number
        m (int): Modulus

    Raises:
        ValueError: If no modular inverse exists

    Returns:
        int: Modular inverse of a mod m
    """
    d, x, _ = gcd(a, m)
    if d != 1:
        raise ValueError(f"No modular inverse for {a} mod {m}")

    # Ensure positive res
    return (x % m + m) % m

def crt(equations: list[Congruence]) -> int:
    """Solves a system of linear congruences using Chinese Remainder Theorem.

    Note that modules must be pairwise coprime!

    Args:
        equations (list[Congruence]): List of congruences to solve

    Returns:
        int: Solution
    """
    M = 1
    for (_, m) in equations:
        M *= m

    res = 0
    for (a, m) in equations:
        Mi = M // m
        Ni = mod_inv(Mi, m)

        res = (res + (a * Mi * Ni) % M) % M

    return res

def main():
    T = int(sys.stdin.readline().strip())
    for _ in range(T):
        a, n, b, m = map(int, sys.stdin.readline().strip().split())

        equations = [Congruence(a, n), Congruence(b, m)]
        res = crt(equations)

        print(f"{res} {n*m}")

if __name__ == "__main__":
    main()