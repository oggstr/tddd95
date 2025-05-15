/**
 * @author Oskar Arensmeier
 * @date 2025-05-02
 */

#include <iostream>

using namespace std;

#define lli long long int
#define llu long long unsigned

/**
 * Combinatorial approach
 * Original alg: https://stackoverflow.com/questions/11891984/count-the-number-of-occurrences-of-0s-in-integers-from-1-to-n
 */
lli count_zeros(lli n)
{
    lli res = 0;
    lli i = 1;

    lli a, b, c;
    while (true) {
        b = n / i;
        c = n % i;
        a = b / 10;
        b = b % 10;

        if (a == 0) {
            break;
        }

        if (b == 0) {
            res += (a - 1) * i + c + 1;
        } else {
            res += a * i;
        }

        i *= 10;
    }

    return res;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    lli n, m;
    do {
        cin >> m >> n;
        if (n == -1 && m == -1) break;

        if (m == 0) {
            cout << count_zeros(n) + 1 << '\n';
        } else {
            cout << count_zeros(n) - count_zeros(m-1) << '\n';
        }
    } while (true);

    cout << flush;

    return 0;
}