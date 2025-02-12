"""
@author Oskar Arensmeier
@date 2025-02-06
"""
from functools import reduce
from math import gcd
import sys


sys.set_int_max_str_digits(999999999)

for frac in open(0):

    frac = frac.strip()
    if frac == "0":
        break

    frac = frac[:-3]
    num  = frac[2:]

    n = len(num)

    # Find nominator and denominator without
    # assuming what portion is repeating
    solutions = map(
        lambda i: (
                int(num) - int(num[:-i] if i < n else 0),
                10**n - 10**(n-i),
            ),
        range(1, n+1)
    )

    # Filter solutions that don't work
    """ solutions = filter(
        lambda pq:
            pq[0]/pq[1] > float(frac),
        solutions
    ) """

    # normalize
    solutions = map(
        lambda pq:
            (pq[0]//gcd(pq[0], pq[1]), pq[1]//gcd(pq[0], pq[1])),
        solutions
    )

    # Find the smallest denominator
    solution = reduce(lambda a, b: a if a and a[1] < b[1] else b, solutions, None)

    # Number is non-repeating fraction
    if not solution:
        d = gcd(int(num), int(pow(10, n)))
        solution = (int(num)//d, int(pow(10, n))//d)

    print(f"{solution[0]}/{solution[1]}")