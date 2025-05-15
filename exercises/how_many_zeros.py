import sys

def count_zeros(
    n: int,
    pos: int,
    digits: int,
    limit: bool,
    has_leading_digit: bool
) -> int:
    result = 0

    if pos == 0:
        return 0

    if limit or pos == digits:
        result += count_zeros(n, pos - 1, digits, limit, has_leading_digit)

    


if __name__ == "__main__":
    #main()
    
    print(count_zeros(1))
    print(count_zeros(10))
    print(count_zeros(100))
    print(count_zeros(1000))
    print(count_zeros(10000))
    print(count_zeros(100000))