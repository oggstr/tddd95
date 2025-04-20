/**
 * @author Oskar Arensmeier
 * @date 2025-04-15
 */

#include <iostream>
#include <vector>
#include <algorithm>

#define ll long long

using namespace std;

ll N, M;

ll divisor_count(ll n, ll d)
{
    ll count = 0;
    while (n > 0) {
        n /= d;
        count += n;
    }
    return count;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    while (cin >> N >> M) {
        if (M == 0) {
            cout << "0 does not divide " << N << "!" << '\n';
            continue;
        }

        ll m = M;
        bool divisible = true;
        for (ll d = 2; d*d <= M; ++d) {

            if (m % d != 0)
                continue;

            ll count = 0;
            while (m % d == 0) {
                m /= d;
                ++ count;
            }

            if (divisor_count(N, d) < count) {
                divisible = false;
                break;
            }
        }

        // m is itself prime
        if (m > 1 && divisible) {
            if (divisor_count(N, m) < 1) {
                divisible = false;
            }
        }

        if (divisible) {
            cout << M << " divides " << N << "!" << '\n';
        } else {
            cout << M << " does not divide " << N << "!" << '\n';
        }
    }
}