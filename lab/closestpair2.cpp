#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

/**
 * @author: Oskar Arensmeier
 * @date: 2025-05-28
 */

/**
 * Finding closest pair of points in 2D
 * Reference: https://cp-algorithms.com/geometry/nearest_points.html
 *
 * Algorithm:
 *
 * 1. Sort all points by x-coordinate
 * 2. Recurse on left/right halves to find min distances
 * 3. Merge halves by y-coord in O(n)
 * 4. Collect points within min_dist of midline into a strip
 * 5. For each strip point, compare against up to 7 neighbors by y
 *
 * Time complexity:
 * - O(n log n)
 * Space complexity:
 * - O(n) extra (temporary array for merge/strip)
 */

using namespace std;
using ll = long long;

const double EPS = 10e-9;
const double INF = 1e18;

/**
 * Point in 2D
 */
struct pt {
    double x, y;
    ll id;
};

struct cmp_x {
    bool operator()(const pt &a, const pt &b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

struct cmp_y {
    bool operator()(const pt &a, const pt &b) const {
        return a.y < b.y;
    }
};

/**
 * Compute Euclidean distance between points a and b
 */
double dist(pt & a, pt & b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

/**
 * Recursive divide-and-conquer routine
 */
void rec(
    vector<pt> & pts,
    int l,
    int r,
    vector<pt> & temp,
    double &min_dist,
    pair<pt,pt> & best
) {
    // Base case
    if (r - l <= 3) {
        for (int i = l; i < r; ++i) {
            for (int j = i + 1; j < r; ++j) {
                double d = dist(pts[i], pts[j]);
                if (d < min_dist) {
                    min_dist = d;
                    best = {pts[i], pts[j]};
                }
            }
        }

        // sort by y for merge
        sort(pts.begin() + l, pts.begin() + r, cmp_y());
        return;
    }

    int m = (l + r) / 2;
    double mid_x = pts[m].x;

    // recurse left and right
    rec(pts, l, m, temp, min_dist, best);
    rec(pts, m, r, temp, min_dist, best);

    // merge left and right halves by y
    merge(pts.begin() + l, pts.begin() + m,
          pts.begin() + m, pts.begin() + r,
          temp.begin(), cmp_y());
    copy(temp.begin(), temp.begin() + (r - l), pts.begin() + l);

    int tsz = 0;
    for (int i = l; i < r; ++i) {

        if (abs(pts[i].x - mid_x) >= min_dist) {
            continue;
        }

        for (int j = tsz - 1; j >= 0 && (pts[i].y - temp[j].y) < min_dist; --j) {
            double d = dist(pts[i], temp[j]);
            if (d < min_dist) {
                min_dist = d;
                best = {pts[i], temp[j]};
            }
        }

        // 
        temp[tsz++] = pts[i];
    }
}

/**
 * Find closest pair of points
 * using divide-and-conquer
 * @param pts Points
 * @return Pair of closest points
 */
pair<pt, pt> closest_pair(vector<pt> &pts)
{
    // sort x for dividing
    sort(pts.begin(), pts.end(), cmp_x());

    vector<pt> temp(pts.size());
    pair<pt,pt> best;
    double min_dist = INF;

    rec(pts, 0, pts.size(), temp, min_dist, best);
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while (true) {
        ll n;
        cin >> n;
        if (n == 0) break;

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
        } catch (const invalid_argument &) {
            cout << "Should be impossible :)\n";
        }
    }

    return 0;
}