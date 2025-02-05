/**
 * @author: Oskar Arensmeier
 * @data: 2025-02-01
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <algorithm>

using namespace std;

class Set : public set<int> {
// public:
//     bool operator==(const Set &other) const {
//         if (size() != other.size()) {
//             return false;
//         }
// 
//         for (int elem : other) {
//             if (find(elem) == end()) {
//                 return false;
//             }
//         }
// 
//         return true;
//     }
};

class SetHash {
public:
    hash<int> hasher;
    size_t operator()(const Set &s) const {
        size_t res = hasher(0);
        for (int i : s) {
            res ^= hasher(i);
        }
        return res;
    }
};

vector<int> solve() {
    vector<int> res;

    int N;
    cin >> N;

    //unordered_map<Set, int, SetHash> lookup;
    map<Set, int> lookup;
    vector<Set> stack;

    for (int i = 0; i < N; ++i) {
        string cmd;
        cin >> cmd;

        if (cmd == "PUSH") {
            stack.push_back(Set());
        }
        else if (cmd == "DUP") {
            stack.push_back(Set(stack.back()));
        }
        else if (cmd == "UNION") {
            Set a = stack.back();
            stack.pop_back();

            Set b = stack.back();
            stack.pop_back();

            Set c;
            set_union(
                a.begin(), a.end(),
                b.begin(), b.end(),
                inserter(c, c.begin())
            );

            stack.push_back(c);
        }
        else if (cmd == "INTERSECT") {
            Set a = stack.back();
            stack.pop_back();

            Set b = stack.back();
            stack.pop_back();

            Set c;
            set_intersection(
                a.begin(), a.end(),
                b.begin(), b.end(),
                inserter(c, c.begin())
            );

            stack.push_back(c);
        }
        else if (cmd == "ADD") {
            Set a = stack.back();
            stack.pop_back();

            Set & b = stack.back();

            // Use id of a if it's known
            if (lookup.find(a) != lookup.end()) {
                b.insert(lookup[a]);
            }
            // Create new id for a
            else {
                int id = lookup.size();
                lookup[a] = id;
                b.insert(id);
            }
        } else {
            cout << "Errrr\n";
        }

        res.push_back(stack.back().size());
    }

    return res;
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    for (int i = 0; i < T; i++) {
        vector<int> res = solve();
        for (int j = 0; j < res.size(); j++)
        {
            cout << res[j] << '\n';
        }

        cout << "***\n";
    }
    cout << flush;

    return 0;
}