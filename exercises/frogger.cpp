/**
 * @author Oskar Arensmeier
 * @date 2024-02-16
 */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

using ll = long long;
using llu = long long unsigned;

struct State {
    int x;
    int y;

    int time;
    int steps;
    int cost;

    State(int x, int y, int time, int steps, int cost)
        : x(x), y(y), time(time), steps(steps), cost(cost) {}

    bool operator==(const State& other) const {
        return (
            x == other.x &&
            y == other.y &&
            time == other.time
        );
    }

    bool operator<(const State& other) const {
        return cost > other.cost;
    }
};

struct hash_state {
    size_t operator()(const State& state) const {
        return hash<size_t>()(
            (size_t) (state.time) +                // time <= 50
            ((size_t) state.x)    * (1llu << 20) + // 1 <= x <= 50
            ((size_t) state.y)    * (1llu << 40)   // 1 <= y <= 20
        );
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(nullptr);

    int N;
    cin >> N;

    for (int i = 0; i < N; ++i) {
        int MAX_STEPS, ROWS, COLS;
        cin >> MAX_STEPS >> ROWS >> COLS;

        ROWS = ROWS + 2;

        vector<vector<char>> lanes(ROWS, vector<char>(COLS, '\0'));
        for (int y = 0; y < ROWS; ++y) {
            for (int x = 0; x < COLS; ++x) {
                cin >> lanes[y][x];
            }
        }
        reverse(lanes.begin(), lanes.end());

        pair<int, int> start = {find(lanes[0].begin(), lanes[0].end(), 'F') - lanes[0].begin(), 0};
        pair<int, int> goal  = {find(lanes[ROWS - 1].begin(), lanes[ROWS - 1].end(), 'G') - lanes[ROWS - 1].begin(), ROWS - 1};

        vector<pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {0, 0}};

        priority_queue<State> pq;
        unordered_set<State, hash_state> visited;

        auto H = [&](int x, int y) {
            return abs(x - goal.first) + abs(y - goal.second);
        };

        State start_state(start.first, start.second, 0, 0, H(start.first, start.second));
        visited.insert(start_state);
        pq.push(start_state);

        int solution = -1;
        while (!pq.empty()) {
            State curr = pq.top();
            pq.pop();

            if (curr.x == goal.first && curr.y == goal.second) {
                solution = curr.steps;
                break;
            }

            for (auto & dir : directions) {
                int new_x = curr.x + dir.first;
                int new_y = curr.y + dir.second;

                if (new_x < 0 || new_x >= COLS || new_y < 0 || new_y >= ROWS) {
                    continue;
                }

                /**
                 * NOTE: Instead of moving the cars, we can move the frog
                 * when checking for collisions. For uneven lanes cars move right
                 * so the frog must move left. For even lanes its the opposite.
                 *
                 * time % 2 == 1: Offset is negative
                 * time % 2 == 0: Offset is positive
                 */
                int offset_x; 
                offset_x = (curr.time + 1) % COLS;
                offset_x = ((new_y % 2) == 0) ? new_x + offset_x : new_x - offset_x;
                offset_x = (offset_x + COLS) % COLS;

                if (lanes[new_y][offset_x] == 'X') {
                    continue;
                }


                /**
                 * NOTE: Time is cyclic as the problem state
                 * space repeats every COLS steps.
                 */
                int next_time  = (curr.time + 1) % (COLS);
                int next_steps = curr.steps + 1;
                int next_cost  = next_steps + H(new_x, new_y);

                if (next_steps > MAX_STEPS) {
                    continue;
                }

                State new_state(new_x, new_y, next_time, next_steps, next_cost);
                if (visited.find(new_state) != visited.end()) {
                    continue;
                }

                visited.insert(new_state);
                pq.push(new_state);
            }
        }

        if (solution == -1) {
            cout << "The problem has no solution.\n";
        } else {
            cout << "The minimum number of turns is " << solution << ".\n";
        }
    }

    cout << flush;
    return 0;
}