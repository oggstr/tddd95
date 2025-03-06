/**
 * @author Oskar Arensmeier
 * @date 2024-02-26
 */
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct edge {
    int u;
    int v;
    double w;
};

pair<int, vector<pair<double, double>>> parse_input()
{
    int M;
    cin >> M;

    vector<pair<double, double>> points;
    points.reserve(M);

    for (int i = 0; i < M; ++i) {
        double a, b;
        cin >> a >> b;
        points.push_back({a, b});
    }

    return {M, points};
}

int find(vector<int> & uf, int x)
{
    if (uf[x] == x) {
        return x;
    }

    return uf[x] = find(uf, uf[x]);
}

bool join(vector<int> & uf, int x, int y)
{
    x = find(uf, x);
    y = find(uf, y);

    if (x == y)
        return false;

    uf[x] = y;
    return true;
}

double solve()
{
    auto [M, points] = parse_input();

    vector<edge> edges;
    edges.reserve(M*M);

    for (int i = 0; i < M; ++i) {
        for (int j = i + 1; j < M; ++j) {
            double dist = sqrt(
                pow(points[i].first - points[j].first, 2) +
                pow(points[i].second - points[j].second, 2)
            );
            edges.push_back({i, j, dist});
        }
    }
    sort(edges.begin(), edges.end(), [](edge const & a, edge const & b) {
        return a.w < b.w;
    });

    vector<int> uf;
    uf.reserve(M);
    for (int i = 0; i < M; ++i) {
        uf.push_back(i);
    }

    double result = 0.0;
    int count = 0;
    for (auto & e : edges) {
        if (find(uf, e.u) != find(uf, e.v)) {
            join(uf, e.u, e.v);

            result += e.w;
            count += 1;

            if (count == M - 1) {
                break;
            }
        }
    }

    return result;
}

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int N;
    cin >> N;

    for (int i = 0; i < N; ++i) {
        cout << setprecision(13) << solve() << '\n';
    }

    cout << flush;
}