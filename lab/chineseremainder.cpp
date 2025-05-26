#include <iostream>
#include <vector>
#include <numeric>

using namespace std;
using ll = long long;

struct Congruence {
    ll a;
    ll m;
};

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

ll mod_inv(ll a, ll n)
{
    ll x, y;
    ll d = gcd(a, n, x, y);
    if (d != 1) {
        throw invalid_argument("Inverse does not exist");
    }

    // Ensure positive result
    return (x % n + n) % n;
}

/**
 * Multiply a and b under mod m using
 * intermediary results that are in O(2m) space.
 * 
 * @param a First number
 * @param b Second number
 * @param m Modulo
 */
ll mult_mod(ll a, ll b, ll m)
{
    /* 
    a * b
    = a * (2k + d)
    = a * 2k + a * d
    */

    ll k, d;
    k = b / 2;
    d = b & 1;

    return ((a * 2 * k) % m +
            (a * d) % m
            ) % m;
}

ll crt(vector<Congruence> & congs)
{
    ll M = 1;
    for (auto & c : congs) {
        M *= c.m;
    }

    ll sol = 0;
    for (auto & c : congs) {
        ll ai = c.a;
        ll Mi = M / c.m;
        ll Ni = mod_inv(Mi, c.m);


        sol = (sol + mult_mod(mult_mod(ai, Mi, M), Ni, M)) % M;
        //sol = (sol + (ai * Mi % M) * Ni) % M;
    }

    return sol;
}

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int T;
    cin >> T;

    ll a, n, b, m;
    for (int i = 0; i < T; ++i) {
        cin >> a >> n >> b >> m;

        vector<Congruence> congruences = {
            {a, n},
            {b, m}
        };

        ll result = crt(congruences);
        cout << result << ' ' << n*m << '\n';
    }
}