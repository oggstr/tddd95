/**
 * @author Oskar Arensmeier
 * @date 2024-02-12
 */
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <inttypes.h>
#include <list>

using namespace std;

// Avoid padding in struct
#pragma pack(push, 1)
struct State {
    // Low 32 bits (6*5 = 30)
    int32_t state_32;
    // High 8 bits
    int8_t  state_8;

    State() : state_32(0), state_8(0) {}
    State(int32_t state_32, int8_t state_8) : state_32(state_32), state_8(state_8) {}

    bool operator==(const State & other) const
    {
        return state_32 == other.state_32 && state_8 == other.state_8;
    }

    State add(int i)
    {
        int32_t new_state_32 = state_32;
        int8_t  new_state_8  = state_8;
        if (i < 5) {
            new_state_32 += (1 << (6*i));
        } else {
            new_state_8 += 1;
        }

        return State(new_state_32, new_state_8);
    }

    void print_state()
    {
        for (int i = 0; i < 5; ++i) {
            cout << ((state_32 >> (6*i)) & 0b111111) << " ";
        }
        cout << (int) state_8 << endl;
    }
};
#pragma pack(pop)

struct StateHash {
    size_t operator()(const State & state) const
    {
        uint64_t combined = (static_cast<uint64_t>(state.state_32)) | (static_cast<uint64_t>(state.state_8) << 32);
        return hash<uint64_t>{}(combined);
    }
};

inline bool should_cache(const State & s)
{
    return true;
    size_t sum = 0;
    for (int i = 0; i < 5; ++i) {
        sum += (s.state_32 >> (6*i)) & 0b111111;
    }
    sum += s.state_8;

    // Cache only 3/4 of the states
    return (sum % 20 < 15);
}

bool dfs(
    unordered_map<State, bool, StateHash> & memo,
    vector<double> & transitions,
    State state,
    double value,
    bool is_nils
) {
    /**
     * NOTE: The state_key tracks how many times each transition has been taken.
     *
     * We can compute the max number of times a transition can be taken by
     * 500 * 0.9 ^ 59 ≈ 1 meaning we need at most 6 bits to represent how many times a transition has been taken.
     *
     * Since we can have at most 6 transitions, we need 6 * 6 = 36 bits to represent the state_key.
     */
    if (memo.find(state) != memo.end()) {
        return memo[state];
    }

    if (transitions[0] * value <= 1.0) {
        if (should_cache(state)) {
            memo[state] = is_nils;
        }
        return is_nils;
    }

    for (int i = 0; i < transitions.size(); i++) {

        State new_state = state.add(i);
        if (dfs(memo, transitions, new_state, value * transitions[i], !is_nils) == is_nils) {

            if (should_cache(state)) {
                memo[state] = is_nils;
            }

            return is_nils;
        }
    }

    if (should_cache(state)) {
        memo[state] = !is_nils;
    }
    return !is_nils;
}

int main() {
    int n;

    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 0; i < n; i++) {
        unordered_map<State, bool, StateHash> memo;

        double start;
        int k;

        cin >> start;
        cin >> k;

        vector<double> transitions(k);
        for (int j = 0; j < k; j++) {
            cin >> transitions[j];
        }

        sort(transitions.begin(), transitions.end(), less<double>());

        if (dfs(memo, transitions, State(0, 0), start, true)) {
            cout << "Nils\n";
        } else {
            cout << "Mikael\n";
        }

        //cout << "Cache size: " << memo.size() << endl;
        //cout << flush;
    }

    cout << flush;
    return 0;
}