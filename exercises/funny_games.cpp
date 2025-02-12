/**
 * @author Oskar Arensmeier
 * @date 2024-02-10
 */
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <list>

using namespace std;

const int NILS_WIN   =  1;
const int MIKAEL_WIN = -1;

//unordered_map<size_t, char> memo;

class LruCache
{
public:
LruCache(size_t capacity) : capacity(capacity) {}

int get(size_t key) {
    lru.remove(key);
    lru.push_front(key);

    return (int) cache[key];
}

bool has(size_t key) {
    cout << "Hit " << key << "Size " << cache.size() << endl;
    return cache.find(key) != cache.end();
}

void put(size_t key, char value) {
    if (cache.size() == capacity) {
        cache.erase(lru.back());
        lru.pop_back();
    }

    cache[key] = value;
    lru.push_front(key);
}

private:
size_t capacity;
unordered_map<size_t, char> cache;
list<size_t> lru;

};

int dfs(
    LruCache & cache,
    vector<double> & transitions,
    size_t state_key,
    double state,
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
    if (cache.has(state_key)) {
        return cache.get(state_key);
    }


    if (state <= 0.0) {
        return is_nils ? MIKAEL_WIN : NILS_WIN;
    }

    if (is_nils) {
        int best = -2;
        for (int i = 0; i < transitions.size(); i++) {
            best = max(
                best,
                dfs(
                    cache,
                    transitions,
                    state_key + (1 << 6*i),
                    state + transitions[i],
                    !is_nils
                )
            );
        }

        cache.put(state_key, best);
        return best;
    } else {
        int best = 2;
        for (int i = 0; i < transitions.size(); i++) {
            best = min(
                best,
                dfs(
                    cache,
                    transitions,
                    state_key + (1 << 6*i),
                    state + transitions[i],
                    !is_nils
                )
            );
        }

        cache.put(state_key, best);
        return best;
    }
}

int main() {
    int n;

    cin >> n;
    for (int i = 0; i < n; i++) {
        //unordered_map<size_t, char> memo;
        LruCache cache(10000000000);

        double start;
        int k;

        cin >> start;
        cin >> k;

        vector<double> transitions(k);
        for (int j = 0; j < k; j++) {
            cin >> transitions[j];
        }

        start = log(start);

        transform(transitions.begin(), transitions.end(), transitions.begin(), [](auto t) {
            return log(t);
        });

        //int result = alpha_beta_search(transitions, 0, start, true, -2, 2);
        int result = dfs(cache, transitions, 0, start, true);

        if (result == NILS_WIN) {
            cout << "Nils\n";
        } else {
            cout << "Mikael\n";
        }

        cout << flush;
    }
}