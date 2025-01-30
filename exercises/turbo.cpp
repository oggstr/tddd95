/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-29
 */

#include <iostream>

using namespace std;

int N;
int arr [100001] = {0};
int pos [100001] = {0};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;

    for (int i = 1; i <= N; ++i) {
        cin >> arr[i];
        pos[arr[i]] = i;
    }

    for (int i = 1; i <= N; ++i) {

        if (i % 2 == 1) {

            int curr  = i/2 + 1;
            int moves = 0;

            for (int j = pos[curr]; j > i/2 + 1; --j) {
                pos[arr[j-1]]++;
                (arr[j] ^= arr[j-1]), (arr[j-1] ^= arr[j]), (arr[j] ^= arr[j-1]);
                ++moves;
            }

            cout << moves << '\n';

        } else {

            int curr  = N - (i/2) + 1;
            int moves = 0;

            for (int j = pos[curr]; j < N - (i/2) + 1; ++j) {
                pos[arr[j+1]]--;
                (arr[j] ^= arr[j+1]), (arr[j+1] ^= arr[j]), (arr[j] ^= arr[j+1]);
                ++moves;
            }

            cout << moves << '\n';

        }
    }

    cout << flush;

    return 0;
}