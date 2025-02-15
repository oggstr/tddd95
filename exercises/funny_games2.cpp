/**
 * @author Oskar Arensmeier
 * @date 2024-02-14
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>

#define SCALE 1e7

using namespace std;

using llu = long long unsigned;
using ll  = long long int;

inline ll scale(double x)
{
    return (ll) round(x * SCALE);
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        double X;
        int K;

        cin >> X;
        cin >> K;

        vector<double> F(K);
        for (auto & f : F) {
            cin >> f;
        }

        /**
         * Taking the log of values allows us to work with ints instead of doubles.
         * SCALE defines the precision of the log values.
         */

        ll start = scale(log2(X));

        sort(F.begin(), F.end(), less<double>());

        vector<ll> costs(K);
        transform(F.begin(), F.end(), costs.begin(), [](double f) { return scale(-log2(f)); });

        vector<bool> dp(start + 1, false);
        dp[0] = false;

        for (ll curr = 1; curr <= start; curr++) {

            bool win = false;
            for (ll j = 0; j < K; j++) {

                // Current player can win directly
                if (curr - costs[j] <= 0) {
                    win = true;
                    break;
                }

                // We can make a move that reaches a state where the opponent loses
                if (!dp[curr - costs[j]]) {
                    win = true;
                    break;
                }
            }

            dp[curr] = win;
        }

        cout << (dp[start] ? "Nils\n" : "Mikael\n");
    }

    cout << flush;
    return 0;
}