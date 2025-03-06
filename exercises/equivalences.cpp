/**
 * @author Oskar Arensmeier
 * @date 2024-02-22
 */

/**
 * Solution:
 *
 * We want to figure out of many edges need to be added for the graph to become strongly connected.
 * First we condense the graph into SCC (Strongly Connected Components).
 * After this we just need to find the number of nodes (condensed graph) with in-degree 0 and out-degree 0. And the the max of the two.
 *
 * We know that we must have at least one out going and one incoming edge to each SCC.
 * In fact the optimal solution will be to add edges exactly such that this holds.
 * Hence we can just count the number of SCC with in-degree 0 and out-degree 0 and the the max.
 * (Obv. this doesn't find how to draw edges, just the number needed.)
 */

#include <iostream>
#include <vector>
#include <set>

using namespace std;

int T;
int N, M;

void dfs(
    int v,
    vector<vector<int>> const & graph,
    vector<bool> & visited,
    vector<int> & output
) {
    visited[v] = true;

    for (auto u : graph[v]) {
        if (visited[u])
            continue;

        dfs(u, graph, visited, output);
    }

    output.push_back(v);
}

pair<vector<vector<int>>, vector<int>> scc_graph(
    vector<vector<int>> const & graph,
    vector<vector<int>> const & graph_t
) {
    vector<int> order;

    vector<bool> visited(N, false);
    for (int v = 0; v < N; ++v) {
        if (visited[v])
            continue;

        dfs(v, graph, visited, order);
    }

    reverse(order.begin(), order.end());

    vector<int> roots(N, 0);
    visited.assign(N, false);
    for (auto v : order) {
        if (visited[v])
            continue;

        vector<int> component;
        dfs(v, graph_t, visited, component);
        int root = *min_element(component.begin(), component.end());


        for (auto u : component)
            roots[u] = root;
    }

    // condensed graph
    vector<vector<int>> graph_c;
    graph_c.assign(N, {});

    for (int v = 0; v < N; ++v) {
        for (auto u : graph[v]) {
            if (roots[v] != roots[u])
                graph_c[roots[v]].push_back(roots[u]);
        }
    }

    return {graph_c, roots};
}

int solve()
{
    vector<vector<int>> graph;
    vector<vector<int>> graph_t;

    graph.assign(N, {});
    graph_t.assign(N, {});

    int v, u;
    for (int i = 0; i < M; ++i) {
        cin >> v >> u;

        graph[v-1].push_back(u-1);
        graph_t[u-1].push_back(v-1);
    }

    auto [graph_c, roots] = scc_graph(graph, graph_t);
    set<int> unique_roots(roots.begin(), roots.end());

    vector<int> in_degree(N, 0);
    vector<int> out_degree(N, 0);

    for (auto v : unique_roots) {
        for (auto u : graph_c[v]) {

            ++out_degree[v];
            ++in_degree[u];
        }
    }

    int in_degree_zero = 0;
    int out_degree_zero = 0;
    for (auto v : set(roots.begin(), roots.end())) {
        if (in_degree[v] == 0)
            ++in_degree_zero;

        if (out_degree[v] == 0)
            ++out_degree_zero;
    }

    // Edge case: The graph was already a single SCC
    if (unique_roots.size() == 1)
        return 0;

    return max(in_degree_zero, out_degree_zero);
}

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    cin >> T;

    for (int i = 0; i < T; ++i) {
        cin >> N >> M;
        cout << solve() << '\n';
    }

    cout << flush;
    return 0;
}