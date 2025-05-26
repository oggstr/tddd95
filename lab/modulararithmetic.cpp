#include <iostream>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-25
 */

/**
 * Modular arithmetic implementation
 *
 * Algorithm:
 *
 * More or less a direct implementation of the math asked
 * in the problem statement. We take care to avoid overflow
 * by avoiding large intermediate results.
 *
 * The only notable part is the division operation. Here
 * we exploit some mathematical re-writing of the problem,
 * and the fact that the extended Euclidean algorithm
 * can be used to find the inverse of a number modulo d.
 *
 * Time complexity:
 *
 * Dominated by the division op, and the extended Euclidean algorithm.
 * Time complexity is O(log(min(x, y))), which in our case is:
 *  - O(log(min(a, d)))
 *
 * Space complexity:
 *  - O(1)
 */

using namespace std;
using ll = long long;

/**
 * Calculate GCD by Extended Euclidean Algorithm:
 * ax + by = gcd(a, b)
 *
 * @param a Numer a
 * @param b Number b
 * @param x Return coeff for a
 * @param y Return coeff for b
 * @return GCD of a and b
 */
ll gcd(ll a, ll b, ll& x, ll& y)
{
    // Base case
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    ll x1, y1;
    ll d = gcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - y1 * (a / b);

    return d;
}

/**
 * a + b mod d
 */
ll div_add(ll a, ll b, ll d)
{
    return (((a % d) + (b % d)) % d + d) % d;
}

/**
 * a - b mod d
 */
ll div_sub(ll a, ll b, ll d)
{
    return (((a % d) - (b % d)) % d + d) % d;
}

/**
 * a * b mod d
 */
ll prod_mod(ll a, ll b, ll d)
{
    return (((a % d) * (b % d)) % d + d) % d;
}

/**
 * a / b mod d
 */
ll div_mod(ll a, ll b, ll d)
{
    /* 
    a / b mod d is equivalent to asking
    for k where bk ≡ a (mod d).

    Which is defined when gcd(b, d) = 1.
    */

    ll k, _;
    ll g = gcd(b, d, k, _);
    if (g != 1) {
        // No solution
        return -1;
    }

    // k is the inverse of b mod d
    return (((k * (a % d)) % d) + d) % d;
}


int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    ll n, d;
    while (1) {
        cin >> d >> n;

        if (n == 0 && d == 0) {
            break;
        }
        
        ll a, b;
        char op;
        for (ll i = 0; i < n; ++i) {

            cin >> a >> op >> b;
            switch (op) {
                case '+':
                cout << div_add(a, b, d) << '\n';
                break;
                case '-':
                cout << div_sub(a, b, d) << '\n';
                break;
                case '*':
                cout << prod_mod(a, b, d) << '\n';
                break;
                case '/':
                cout << div_mod(a, b, d) << '\n';
                break;
            }
        }
    }

    cout << flush;
    return 0;
}