/**
 * @author: Oskar Arensmeier
 * @date: 2025-02-04
 */

#include <iostream>
#include <string>

using namespace std;

typedef long long unsigned llu;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    llu sol = 1;
    for (llu i = 1; i <= n; i++)
    {
        sol *= i;
        while (sol % 10 == 0)
        {
            sol /= 10;
        } 
        sol %= 1000000000000;
    }

    string sol_str = to_string(sol);

    if (sol_str.size() < 4) {
        cout << sol_str << endl;
    } else {
        cout << sol_str.substr(sol_str.size() - 3) << endl;
    }

    return 0;
}