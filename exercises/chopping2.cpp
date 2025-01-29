/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-27
 */
#include <cstdio>
#include <vector>
#include <sstream>
#include <iostream>
#include <queue>

using namespace std;

int N;
int v [200002] = {0};
int u [200002] = {0};
int neighbor_count [200002] = {0};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;

    for (int i = 1; i <= N; ++i) {
        cin >> v[i];
        neighbor_count[v[i]]++;

        // Valid tree requires exactly N-1 edges
        if (i == N && v[i] != N + 1) {
            cout << "Error\n";
            cout << flush;
            return 0;
        }
    }

    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 1; i <= N; ++i) {
        if (neighbor_count[i] == 0) {
            pq.push(i);
        }
    }

    for (int i = 1; i <= N; ++i) {
        int node = pq.top();
        pq.pop();

        u[i] = node;
        neighbor_count[v[i]]--;

        if (neighbor_count[v[i]] == 0) {
            pq.push(v[i]);
        }

        if (pq.empty() && i < N) {
            cout << "Error\n";
            cout << flush;
            return 0;
        }
    }

    for (int i = 1; i <= N; ++i) {
        cout << u[i] << "\n";
    }

    cout << flush;

    return 0;
}