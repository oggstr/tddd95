/**
 * @author: Oskar Arensmeier
 * @data: 2024-01-31
 */
#include <iostream>
#include <vector>
#include <bitset>
#include <queue>
#include <climits>

using namespace std;

#define oo INT_MAX

int C;
int N;
int A;

int input [100001] = {0};
queue<int> next_access [100001];
bitset <100001> cache;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> C >> N >> A;

    // Read input and initialize next_access
    for (int i = 0; i < A; ++i) {
        cin >> input[i];
        next_access[input[i]].push(i);
    }

    // Add oo (INT_MAX) as last elem for all next_access queues
    for (int i = 0; i < N; ++i) {
        next_access[i].pop();
        next_access[i].push(oo);
    }

    int misses = 0;
    int items_cached = 0;

    // Priority queue for cache eviction
    // Stores the index (into input) of next accesses
    priority_queue<int, vector<int>, less<int>> pq;
    for (int i = 0; i < A; ++i) {

        // Hit
        if (cache[input[i]]) {
            // Push next access to the queue
            // (ignores that this index is already in the queue - but that's fine)
            pq.push(next_access[input[i]].front());
            next_access[input[i]].pop();
            continue;
        }

        // Miss
        misses++;

        // Cache item
        cache[input[i]] = 1;

        // cache not full
        if (items_cached < C) {
            items_cached++;

        // cache full
        } else {
            if (pq.top() != oo) {
                // evict
                cache[input[pq.top()]] = 0;
                pq.pop();
            } else {
                // this item wont be accessed again
                pq.pop();
            }
        }

        // Push next access to the queue
        pq.push(next_access[input[i]].front());
        next_access[input[i]].pop();
    }

    cout << misses << endl;

    return 0;
}