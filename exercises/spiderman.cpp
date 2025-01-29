/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-22
 */

#include <cstdio>
#include <tuple>
#include <bitset>

using namespace std;

#define INF 1000000

typedef unsigned int uint;

// NOTE:
// Any valid solution must start with U and end with D

bitset<40> state(1);
bitset<40> best(0);

int dists[40] = {0};
int best_height = INF;

int N;
int M;

void update_if_better(int max_height)
{
    if (max_height < best_height) {
        best_height = max_height;
        best = state;
    }
}

void dfs(int curr_max_height, int curr_height, int pos)
{
    int d = dists[pos];

    // Base case
    if (pos == M) {

        // Check if valid
        if (curr_height - d == 0) {
            update_if_better(curr_max_height);
        }

        return;
    }

    int up   = curr_height + d; 
    int down = curr_height - d; 

    if (down >= 0) {
        state.set(pos, false);
        dfs(
            curr_max_height,
            down,
            pos+1
        );
    }

    if (up < best_height) {
        state.set(pos, true);
        dfs(
            up > curr_max_height ? up : curr_max_height, // Update curr_max_height if needed
            up,
            pos + 1
        );
    }
}

int main()
{
    scanf("%d", &N);

    for (int i = 0; i < N; ++i) {
        scanf("%d", &M);

        state = bitset<40>(1);
        best  = bitset<40>(0);

        best_height = INF;

        for (int j = 0; j < M; ++j) {
            scanf("%d", &dists[j]);
        }

        // Sanity check: if sum is not divisible by 2,
        // no solution exists
        int sum = 0;
        for (int j = 0; j < M; ++j) {
            sum += dists[j];
        }
        if (sum % 2 == 1) {
            printf("IMPOSSIBLE\n");
            continue;
        }

        if (M <= 1) {
            printf("IMPOSSIBLE\n");
            continue;
        }
        if (M == 2) {
            if (dists[0] == dists[1]) {
                printf("UD\n");
            } else {
                printf("IMPOSSIBLE\n");
            }

            continue;
        }

        // Start from 1 instead of 0 since
        // we must always go up at the start
        dfs(dists[0], dists[0], 1);

        if (best == 0) {
            printf("IMPOSSIBLE\n");
            continue;
        }

        for (int j = 0; j < M; ++j) {
            if (best.test(j)) {
                printf("U");
            } else {
                printf("D");
            }
        }

        printf("\n");
    }

    return 0;
}