/**
 * @author: Oskar Arensmeier
 * @data: 2025-02-02
 */

#include <vector>
#include <climits>
#include <iostream>

// NOTE: int max is 2147483647, but negative min is -2147483648
// To find the solution for a negative number, it is enough to find a positive
// solution and check that the exponent is uneven. To do so, we must not restrict
// the max value to 2147483647, but to 2147483648, which is 2^31.
#define MAX 2147483648 // 2^31

using namespace std;

typedef long long int lli;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<lli>> table;

    // 46341^2 = 2147488281 > 2147483648
    for (lli num = 2; num < 46341; ++num) {
        vector<lli> row;

        lli curr = num;
        while (true) {
            curr *= num;
            if (curr > MAX) {
                break;
            }

            row.push_back(curr);
        }

        table.push_back(row);
    }

    auto find_any = [&](lli n) {
        for (int i = 0; i < table.size(); i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if (table[i][j] == n) {
                    return j+2;
                }
            }
        }

        return 1;
    };

    auto find_uneven = [&](lli n) {
        for (int i = 0; i < table.size(); i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if (table[i][j] == n && j % 2 == 1) {
                    return j+2;
                }
            }
        }

        return 1;
    };

    lli n;
    while (1) {
        cin >> n;
        if (n == 0) {
            break;
        }

        if (n < 0) {
            cout << find_uneven(-n) << endl;
        } else {
            cout << find_any(n) << endl;
        }
    }

    cout << flush;
    return 0;
}