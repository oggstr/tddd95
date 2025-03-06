/**
 * @author Oskar Arensmeier
 * @date 2025-03-05
 */

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <climits>
#include <queue>

#define SZ 5000

using namespace std;

map<string, int> names;
map<string, int> parties;
map<string, int> clubs;

vector<int> graph[5000]; // u -> v
int flow[5000][5000];

int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;

    queue<pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty()) {
        int curr      = q.front().first;
        int curr_flow = q.front().second;
        q.pop();

        for (auto next : graph[curr]) {

            if (parent[next] == -1 && flow[curr][next]) {

                parent[next] = curr;
                int new_flow = min(curr_flow, flow[curr][next]);

                if (next == t) {
                    return new_flow;
                }

                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

int max_flow(int s, int t, int n) {
    int curr_flow = 0;
    int new_flow;

    vector<int> parent(n);

    while ((new_flow = bfs(s, t, parent))) {
        curr_flow += new_flow;

        int curr = t;
        while (curr != s) {
            int prev = parent[curr];
            flow[prev][curr] -= new_flow;
            flow[curr][prev] += new_flow;

            curr = prev;
        }
    }

    return curr_flow;
}

int parse()
{
    int N;
    cin >> N;

    int club_count = 0;
    int id = 2;
    for (int i = 0; i < N; ++i) {
        string name, party;
        cin >> name >> party;

        if (parties.find(party) == parties.end()) {
            // source -> party
            graph[0].push_back(id);
            graph[id].push_back(0);

            parties[party] = id;
            id ++;
        }
        
        if (names.find(name) == names.end()) {
            names[name] = id;
            id ++;
        }

        int n = names[name];
        int p = parties[party];

        // party -> name
        graph[p].push_back(n);
        graph[n].push_back(p);
        
        int C;
        cin >> C;
        for (int j = 0; j < C; ++j) {
            string club;
            cin >> club;

            if (clubs.find(club) == clubs.end()) {
                // club -> sink
                graph[id].push_back(1);
                graph[1].push_back(id);

                clubs[club] = id;
                id ++;
            }

            int c = clubs[club];

            // name -> club
            graph[n].push_back(c);
            graph[c].push_back(n);
        }
    }

    int max_party_flow = (clubs.size() - 1) / 2;

    for (int p : graph[0]) {
        // source -> party
        flow[0][p] = max_party_flow;
    }

    for (auto & party: parties) {
        int p = party.second;
        for (auto n : graph[p]) {
            // party -> name
            flow[p][n] = 1;
        }
    }

    for (auto & name : names) {
        int n = name.second;
        for (auto c : graph[n]) {
            // name -> club
            flow[n][c] = 1;
        }
    }

    for (auto & club : clubs) {
        int c = club.second;
        // club -> sink
        flow[c][1] = 1;
    }

    return id;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    for (int t = 0; t < T; ++t) {
        names.clear();
        parties.clear();
        clubs.clear();

        fill(&flow[0][0], &flow[0][0] + SZ * SZ, 0);
        for (int i = 0; i < SZ; ++i) {
            graph[i].clear();
        }

        int count = parse();

        /* for (int i = 0; i < count; ++i) {
            for (int j = 0; j < count; ++j) {
                cout << flow[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << flush; */

        /* for (int u = 0; u < count; ++u) {
            cout << u << ": ";
            for (int v : graph[u]) {
                cout << v << ' ';
            }
            cout << '\n';
        }
        cout << flush; */

        int f = max_flow(0, 1, count);

        if (f != clubs.size()) {
            cout << "Impossible.\n";
        } else {
            for (auto & n: names) {
                for (auto & c: clubs) {
                    if (flow[c.second][n.second] == 1 && flow[n.second][c.second] == 0) {
                        cout << n.first << ' ' << c.first << '\n';
                    }
                }
            }
        }

        cout << '\n';
    }

    cout << flush;
}