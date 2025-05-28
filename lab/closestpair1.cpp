#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <unordered_map>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-28
 */

/**
 * Closest pair of points in 2D
 *
 * Algorithm:
 *
 * Bucket/grid approach. We divide all points into a grid of cells
 * (using a hash map). The size of the grid is at least as large as
 * the minimum distance between two points.
 *
 * After dividing the grid, we simply iterate over all points
 * and check the neighboring cells, and the cell itself, for
 * another point. If we find a point that is closer than the
 * current minimum distance, we update the result.
 *
 * Time complexity:
 *
 * Worst case: O(n^2)
 * Average case: O(n log n), assuming uniform distribution of points!
 *
 * Space complexity: O(n) (Since we use a hash map)
 */

using namespace std;
using ll = long long;

const double EPS = 10e-9;
const double PI  = acos(-1.0);
const double INF = 1e18;

/**
 * Point
 */
struct pt {
    double x;
    double y;
    ll id;
};

struct hash_cell {
    /**
     * Hash func for cell coords
     * @param p Cell coord
     * @return hash val
     */
    size_t operator()(const pair<ll, ll> &p) const {
        auto h1 = hash<ll>{}(p.first);
        auto h2 = hash<ll>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};

/**
 * Calculate squared distance
 * @param a First point
 * @param b Second point
 * @return Dist squared
 */
double dist_sq(pt & a, pt & b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

/**
 * Calculate squared distance
 * @param p Pair points
 * @return Dist squared
 */
double dist_sq(pair<pt, pt> & p) {
    return dist_sq(p.first, p.second);
}

/**
 * Search for closest points in a grid cell
 * @param x Cell x coordinate
 * @param y Cell y coordinate
 * @param grid Grid of points
 * @param curr Current point
 * @param min_dist Minimum distance found so far
 * @param res Result pair of closest points
 */
void search_cell(
    ll x,
    ll y,
    unordered_map<pair<ll, ll>, vector<pt>, hash_cell> & grid,
    pt & curr,
    double & min_dist,
    pair<pt, pt> & res
) {
    // No points in this cell
    if (grid.find({x, y}) == grid.end()) {
        return;
    }

    for (auto & p : grid[{x, y}]) {
        // Skip the same point
        if (curr.id == p.id) {
            continue;
        }

        double d = dist_sq(curr, p);
        if (d < min_dist) {
            min_dist = d;
            res = {curr, p};
        }
    }
}

/**
 * Find closest pair of points
 * @param points Vector of points
 * @return Pair of closest points
 */
pair<pt, pt> closest_pair(vector<pt> &points)
{
    if (points.size() < 2) {
        throw invalid_argument("Too few points");
    }

    double grid_sz = INF;

    // Find a good grid size
    for (size_t i = 1; i < points.size(); ++i) {
        grid_sz = min(grid_sz, dist_sq(points[i], points[i - 1]));
    }
    grid_sz = sqrt(grid_sz);

    // Avoid zero grid size
    if (grid_sz < EPS) {
        grid_sz = 1.0;
    }

    unordered_map<pair<ll, ll>, vector<pt>, hash_cell> grid;
    grid.reserve(points.size()*2);

    for (auto & p: points) {
        ll x = floor(p.x / grid_sz);
        ll y = floor(p.y / grid_sz);
        grid[{x, y}].push_back(p);
    }

    pair<pt, pt> res;
    double min_dist = INF;

    for (auto & p: points) {
        ll x = floor(p.x / grid_sz);
        ll y = floor(p.y / grid_sz);

        for (ll dx = -1; dx <= 1; ++dx) {
            for (ll dy = -1; dy <= 1; ++dy) {
                search_cell(x + dx, y + dy, grid, p, min_dist, res);
            }
        }
    }

    return res;
}

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    ll n;
    while (1) {
        cin >> n;
        if (n == 0) {
            break;
        }

        vector<pt> points(n);
        for (ll i = 0; i < n; ++i) {
            cin >> points[i].x >> points[i].y;
            points[i].id = i;
        }

        try {
            auto res = closest_pair(points);
            cout << fixed << setprecision(2)
                 << res.first.x << ' ' << res.first.y << ' '
                 << res.second.x << ' ' << res.second.y << '\n';
        } catch (const invalid_argument &e) {
            cout << "Should be impossible :)\n";
        }
    }

    cout << flush;
    return 0;
}
