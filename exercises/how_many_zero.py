
def count_zeros(num: int) -> int:
    """
    Count the number of zeros in a given integer.

    :param num: The integer to count zeros in.
    :return: The count of zeros in the integer.
    """
    # Convert the number to a string and count '0's
    return str(num).count('0')

sum = 0
for i in range(306):
    sum += count_zeros(i)

print(f"{sum}")