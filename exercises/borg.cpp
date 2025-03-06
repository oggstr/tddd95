/**
 * @author Oskar Arensmeier
 * @date 2024-02-21
 */

#include <iostream>
#include <vector>
#include <queue>
#include <bitset>
#include <set>
#include <algorithm>

using namespace std;

int N;
int X;
int Y;

using pos      = pair<int, int>;
using pos_dist = pair<pos, int>;

struct edge {
    pos origin;
    pos target;
    int weight;
};

int pos_to_idx(pos p)
{
    return p.second * 50 + p.first;
}

int find(vector<int> & set, int x)
{
    if (set[x] == x) {
        return x;
    }

    return set[x] = find(set, set[x]);
}

bool join(vector<int> & set, int x, int y)
{
    x = find(set, x);
    y = find(set, y);

    if (x == y) {
        return false;
    }

    set[x] = y;
    return true;
}

vector<edge> bfs(
    vector<vector<char>> & board,
    pos source,
    set<pos> & sinks
) {
    auto comp = [] (pos_dist a, pos_dist b) {
        return a.second > b.second;
    };

    queue<pos_dist> q;
    vector<bitset<50>> visited (50, bitset<50>());

    q.push({source, 0});
    visited[source.second][source.first] = true;

    vector<pos> moves = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };

    vector<edge> edges;
    while (!q.empty()) {
        auto [curr_pos, curr_dist] = q.front();
        q.pop();

        if (curr_pos != source && sinks.find(curr_pos) != sinks.end()) {
            edges.push_back({source, curr_pos, curr_dist});
        }

        for (auto move : moves) {
            int x = curr_pos.first + move.first;
            int y = curr_pos.second + move.second;

            if (x < 0 || x >= X || y < 0 || y >= Y) {
                continue;
            }

            if (visited[y][x]) {
                continue;
            }

            if (board[y][x] == '#') {
                continue;
            }

            visited[y][x] = true;
            q.push({{x, y}, curr_dist + 1});
        }
    }

    return edges;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(nullptr);

    cin >> N;


    for (int n = 0; n < N; ++n) {
        cin >> X >> Y;

        vector<vector<char>> board (Y, vector<char>(X, '\0'));
        set<pos> sources;

        string line;
        getline(cin, line); // eat newline

        for (int y = 0; y < Y; ++y) {
            getline(cin, line);
            for (int x = 0; x < X; ++x) {
                board[y][x] = line.size() > x ? line[x] : '#';

                if (board[y][x] == 'S' || board[y][x] == 'A') {
                    sources.insert({x, y});
                }
            }
        }

        vector<edge> edges;
        for (auto source : sources) {
            auto e = bfs(board, source, sources);
            edges.insert(edges.end(), e.begin(), e.end());
        }

        auto sorter = [] (edge a, edge b) {
            return a.weight < b.weight;
        };

        sort(edges.begin(), edges.end(), sorter);

        vector<int> set (50*50);
        for (int i = 0; i < set.size(); ++i) {
            set[i] = i;
        }

        int total = 0;
        for (auto e : edges) {
            int a = pos_to_idx(e.origin);
            int b = pos_to_idx(e.target);

            if (join(set, a, b)) {
                total += e.weight;
            }
        }
        cout << total << '\n';
    }

    cout << flush;
    return 0;
}