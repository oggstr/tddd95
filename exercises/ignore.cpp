/**
 * @author Oskar Arensmeier
 * @date 2025-02-04
 */

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#define SIZE 1000

typedef long long int INT;

char flip_char [10] =
{
    '0', // 0 -> 0
    '1', // 1 -> 1
    '2', // 2 -> 2
    '-', // Does not exist
    '-', // Does not exist
    '5', // 5 -> 5
    '9', // 6 -> 9
    '-', // Does not exist
    '8', // 8 -> 8
    '6'  // 9 -> 6
};

INT to_base_7(INT base10)
{
    INT base7 = 0;
    INT i = 0;
    while (base10 > 0) {
        base7 += (base10 % 7) * pow(10, i);
        base10 /= 7;
        i++;
    }
    return base7;
}

string solve(INT base7)
{
    INT res = 0;
    INT i = 0;
    while (base7 > 0) {
        INT digit = base7 % 10;
        base7 /= 10;

        /** Conversion table from base 7 to solution
         *  0 -> 0
         *  1 -> 1
         *  2 -> 2
         *  3 -> 5
         *  4 -> 6
         *  5 -> 8
         *  6 -> 9
         */
        if (digit == 3 || digit == 4 ) {
            digit += 2;
        } else if (digit == 5 || digit == 6) {
            digit += 3;
        }

        res += digit * pow(10, i);
        i++;
    }

    string res_str = "";
    while (res > 0) {
        res_str += flip_char[res % 10];
        res /= 10;
    }

    return res_str;
}

int main ()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int n;
    while (cin >> n) {
        INT b7 = to_base_7(n);
        cout << solve(b7) << '\n';
    }

    cout << flush;

    return 0;
}