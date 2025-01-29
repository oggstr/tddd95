/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-27
 */
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iostream>

using namespace std;

int N;
int v [200002] = {0};
int lookup [200002] = {0};
int solution [200002] = {0};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    //scanf("%d", &N);
    cin >> N;

    // vector<int> v(N);
    for (int i = 0; i < N; ++i) {
        //scanf("%d", &v[i]);
        cin >> v[i];
        lookup[v[i]] = i;
    }

    if (N == 0) {
        //printf("Error\n");
        cout << "Error\n";
        cout << flush;
        return 0;
    }

    // Valid tree requires exactly N-1 edges
    if (v[N-1] != N + 1) {
        //printf("Error\n");
        cout << "Error\n";
        cout << flush;
        return 0;
    }

    // unordered_map<int, int> lookup;
    // vector<int> lookup (N);
    //for (int i = N-1; i >= 0; --i) {
    //    /* if (lookup.find(v[i]) != lookup.end()) {
    //        continue;
    //    } */
    //    if (lookup[v[i]] != 0) {
    //        continue;
    //    }
    //
    //    lookup[v[i]] = i;
    //}

    // vector<int> solution(N);
    int next_free = -1;
    for (int i = 1; i <= N; ++i) {

        // Find slot for i
        // If i occurs in the input, it must be placed after
        // its last occurrence
        // int slot =  (lookup.find(i) != lookup.end())
        int slot = (lookup[i] != 0)
            ? lookup[i]
            : next_free;

        if (slot < next_free) {
            slot = next_free;
        }

        do {
            slot ++;
        } while (slot < N && solution[slot] != 0);

        if (slot == N) {
            //printf("Error\n");
            cout << "Error\n";
            cout << flush;
            return 0;
        }

        solution[slot] = i;
    }

    //ostringstream oss;
    for (int i = 0; i < N; ++i) {
        /* oss << solution[i] << "\n"; */
        cout << solution[i] << '\n';
    }

    //printf("%s", oss.str().c_str());
    //cout << oss.str();
    cout << flush;

    /*for (int i = 0; i < N; ++i) {
        printf("%d\n", solution[i]);
    } */

    return 0;
}