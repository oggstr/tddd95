/**
 * @author Oskar Arensmeier
 * @date 2024-02-15
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <set>

using namespace std;

using llu = long long unsigned;
using ll  = long long int;


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
            double frac;
            cin >> frac;

            // Convert 1 / F_i since we're computing backwards
            f = 1.0 / frac;
        }

        sort(F.begin(), F.end());

        // Win directly
        if (X <= F.back()) {
            cout << "Nils\n";
            continue;
        }

        set<pair<double, int>> intervals;

        intervals.insert({F.back(), false});

        auto it = intervals.begin();
        while (true) {

            auto curr = *it;

            // Above curr_limit outcome is is_win
            double curr_limit = curr.first;
            bool   is_win     = curr.second;

            // Current player can win
            if (curr_limit * F[0] > X) {
                break;
            }

            if (is_win) {
                it++;
                continue;
            }

            for ( auto & f : F) {
                /**
                 * W: winning
                 * L: Losing
                 * F: False
                 * T: True
                 *
                 * Example:
                 *             | Current limit
                 *             v
                 *        W      L   W      L
                 *    |--------|---|---|----------
                 *             F   T   F
                 *                        ^
                 *             |----------| next_limit_lower
                 *                            ^
                 *                 |----------| next_limit_upper
                 *
                 *   1. We want to indicate that we can win at point next_limit_lower
                 *      1.1 Either this must already be the case
                 *      1.2 Or we must add an indicator that we can win from here
                 *
                 * Result:
                 *             | Current limit
                 *             v
                 *        W      L   W   L   W
                 *    |--------|---|---|--|-------
                 *             F   T   F  T
                 *                             ^
                 *                 |-----------| next_limit_upper
                 *
                 *   2. We want to indicate that we can't win at point next_limit_upper
                 *      2.1 Either this must already be the case
                 *      2.2 Or we must add an indicator that we can't win from here
                 * 
                 * Result:
                 *             | Current limit
                 *             v
                 *        W      L   W   L   W    L
                 *    |--------|---|---|--|----|------
                 *             F   T   F  T    F
                 *
                 * 3. Remove any limits strictly between next_limit_lower and next_limit_upper
                 * 4. Repeat for all weapons
                 */

                double next_limit_lower = curr_limit * f;

                // Find a lower bound for next_limit
                auto lower_bound = prev(intervals.upper_bound({next_limit_lower, true}));

                // If the lower bound is not winning
                if (! lower_bound->second) {

                    // There exists a limit here that is not winning
                    if (lower_bound->first == next_limit_lower) {
                        // Get the one before instead
                        lower_bound = prev(lower_bound);
                    } else {
                        // Add interval saying we can win from next_limit_lower
                        lower_bound = intervals.insert({next_limit_lower, true}).first;
                    }

                }

                double new_limit_upper = next(it)->first * f;
                auto upper_bound = intervals.lower_bound({new_limit_upper, true});

                if (upper_bound == intervals.end() || upper_bound->second) {
                    // There exists a winning limit here - move to the next limit (which by definition is losing)
                    if (upper_bound != intervals.end() && upper_bound->first == new_limit_upper) {
                        upper_bound = next(upper_bound);

                    // No losing limit here - add one
                    } else {
                        upper_bound = intervals.insert({new_limit_upper, false}).first;
                    }
                }


                // Erase all intervals strictly between lower and upper bound
                auto erase = next(lower_bound);
                while (erase != upper_bound) {
                    erase = intervals.erase(erase);
                }
            }

            it++;
        }

        auto outcome = prev(intervals.lower_bound({X, false}));
        if (outcome->second) {
            cout << "Nils\n";
        } else {
            cout << "Mikael\n";
        }
    }

    cout << flush;
    return 0;
}