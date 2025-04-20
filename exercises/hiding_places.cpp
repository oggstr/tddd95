/**
 * @author Oskar Arensmeier
 * @date 2024-03-06
 */

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

#define SZ 8

using namespace std;

int N;

int moves[8][2] = {
    {1, 2},
    {2, 1},
    {2, -1},
    {1, -2},
    {-1, -2},
    {-2, -1},
    {-2, 1},
    {-1, 2}
};

int board [SZ][SZ];

void bfs(int x, int y)
{
    queue<pair<int, int>> q;
    q.push({x, y});

    board[x][y] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int i = 0; i < 8; ++i) {
            int nx = x + moves[i][0];
            int ny = y + moves[i][1];

            if (nx >= 0 && nx < SZ && ny >= 0 && ny < SZ && board[nx][ny] == -1) {
                board[nx][ny] = board[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    for (int i = 0; i < N; ++i) {
        fill(&board[0][0], &board[0][0] + SZ * SZ, -1);

        int x, y;
        string pos;
        cin >> pos;

        y = (int) pos[0] - 'a';
        x = (int) pos[1] - '1';

        bfs(x, y);

        int best = 0;
        for (int i = 0; i < SZ; ++i) {
            for (int j = 0; j < SZ; ++j) {
                best = max(best, board[i][j]);
            }
        }

        cout << best << ' ';
        for (int i = 7; i >= 0; --i) {
            for (int j = 0; j < SZ; ++j) {
                if (board[i][j] == best) {
                    cout << (char) (j + 'a') << (i + 1) << ' ';
                }
            }
        }

        cout << '\n';
    }

    cout << flush;
    return 0;
}