/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-22
 */

#include <unordered_map>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <algorithm>

// #define SIZE 1000000000

using namespace std;

// typedef long unsigned uint64;

// Index : need level
// Value : count of that need level 
// uint32_t needs [SIZE] = {0};

int main()
{
    uint32_t M;
    uint32_t N;

    scanf("%u %u", &M, &N);

    unordered_map<uint64_t, uint64_t> needs;

    uint64_t need;         // candy
    uint64_t max_need = 0; // max candy
    uint64_t anger = 0;    // anger lvl

    for (uint64_t i = 0; i < N; ++i) {
        scanf("%u", &need);

        // Count kids with given need
        needs[need] += 1;

        anger += need*need;

        if (need > max_need) {
            max_need = need;
        }
    }

    uint64_t curr = max_need;
    // Keep giving the largest need
    while (M > 0) {
        uint64_t count = needs[curr];

        if (count > M) {
            count = M;
        }

        // Giving 1 candy to need = n
        // will decrease anger by (n + n - 1)
        uint64_t change = count * (curr + curr - 1);
        if (change > anger) {
            anger = 0;
            break;
        }

        anger -= change;
        M     -= count;

        needs.erase(curr);

        // Add new needs
        needs[curr-1] += count;
        curr--;
    }

    printf("%lu\n", anger);

    return 0;
}