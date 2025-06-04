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

Theory:
The Chinese Remainder Theorem states that given a system of linear congruences:
x ≡ a_i (mod m_i)  i = 1,2 ... n

A solution exists if all m_i are pairwise coprime.
The solution can be found using:

x = sum(a_i * M_i * N_i) mod M
where:
- M = prod(m_i) i = 1,2 ... n
- M_i = M / m_i
- N_i = modular inverse of M_i mod m_i

The algorithm computes this solution iteratively.
The modular inverse is found using the Extended Euclidean Algorithm.

Since the the Euclidean algorithm gives us:
a*x + b*y = gcd(a, b)

We can find the modular inverse x, of a mod m from
a*x + m*y = gcd(a, m) = 1, since m and a are coprime

Proof:
a*x + m*y ≡ 1 (mod m)
a*x + 0   ≡ 1 (mod m)
a*x       ≡ 1 (mod m)

Since the Euclidean may return a negative x,
we ensure the result is positive by taking
x = (x % m + m) % m

Algorithm:
1. Compute M = prod(m_i)
2. For each equation (a_i, m_i):
3. Compute M_i = M / m_i
4. Compute N_i = mod_inv(M_i, m_i)
5. Update result: res += (a_i * M_i * N_i)
6. Compute final result: res %= M

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
    """Find modular inverse of:
    a mod m

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

        res += (a * Mi * Ni)

    res %= M
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