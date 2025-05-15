/**
 * @author Oskar Arensmeier
 * @date 2025-04-28
 *
 * @note Something about the Kattis environment makes this arrive at the wrong answer.
 * Local testing works fine....
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define PI 3.1415926535897932384626433832795028841971693993751058
#define EPS 1e-9
#define ld long double

struct pt {
    ld x;
    ld y;

    bool operator==(const pt& other) const {
        return fabsl(x - other.x) < EPS && fabsl(y - other.y) < EPS;
    }

    bool operator<(const pt& other) const {
        return make_pair(y, x) < make_pair(other.y, other.x);
    }
};

int orientation(
    const pt& p,
    const pt& q,
    const pt& r
) {
    ld v = p.x * (q.y - r.y)
             + q.x * (r.y - p.y)
             + r.x * (p.y - q.y);

    if (v < 0) return -1; // clockwise
    if (v > 0) return 1; // counter-clockwise

    // collinear, but not relevant for this problem
    return 0;
}

bool is_cw(
    const pt& p,
    const pt& q,
    const pt& r,
    bool include_collinear
) {
    int o = orientation(p, q, r);
    return o < 0 || (include_collinear && o == 0);
}

bool is_collinear(
    const pt& p,
    const pt& q,
    const pt& r
) {
    return orientation(p, q, r) == 0;
}

vector<pt> convex_hull(
    vector<pt>& points,
    bool include_collinear = false
) {
    pt p_min = *min_element(points.begin(), points.end(), [](const pt& a, const pt& b) {
        return a < b;
    });

    sort(points.begin(), points.end(), [&p_min](const pt& a, const pt& b) {
        int o = orientation(p_min, a, b);
        if (o == 0)
            return (p_min.x-a.x)*(p_min.x-a.x) + (p_min.y-a.y)*(p_min.y-a.y)
                 < (p_min.x-b.x)*(p_min.x-b.x) + (p_min.y-b.y)*(p_min.y-b.y);
        return o < 0;
    });

    if (include_collinear) {
        int i = (int) points.size()-1;
        while (i >= 0 && is_collinear(p_min, points[i], points.back())) i--;
        reverse(points.begin()+i+1, points.end());
    }

    vector<pt> hull;
    for (int i = 0; i < (int) points.size(); i++) {

        while (hull.size() > 1 && ! is_cw(hull[hull.size()-2], hull.back(), points[i], include_collinear)) {
            hull.pop_back();
        }

        hull.push_back(points[i]);
    }

    if (hull.size() == 2 && hull[0] == hull[1]) {
        hull.pop_back();
    }

    return hull;
}

ld area(const vector<pt>& points)
{
    ld res = 0;
    for (size_t i = 0; i < points.size(); i++) {
        pt p = i ? points[i - 1] : points.back();
        pt q = points[i];
        res += (p.x - q.x) * (p.y + q.y);
    }

    return fabsl(res) / 2;
}

int main()
{
    cin.tie(nullptr);
    cout.sync_with_stdio(false);

    int N, n;

    cin >> N;

    for (int i = 0; i < N; ++ i) {
        cin >> n;

        vector<pt> points(4*n);
        ld boards_area = 0.0;
        ld x, y, w, h, v;
        for (int j = 0; j < n; ++ j) {
            cin >> x >> y >> w >> h >> v;

            // Convert to radians
            // and rotate to have angle in positive direction
            // (for a normal right-handed coordinate system)
            v = PI/2 - v * PI / 180.0;

            ld offset_x = w / 2 * cos(v) + h / 2 * sin(v);
            ld offset_y = w / 2 * sin(v) + h / 2 * cos(v);

            pt p1 = {x - offset_x, y - offset_y};
            pt p2 = {x + offset_x, y - offset_y};
            pt p3 = {x + offset_x, y + offset_y};
            pt p4 = {x - offset_x, y + offset_y};

            points[4*j] = p1;
            points[4*j+1] = p2;
            points[4*j+2] = p3;
            points[4*j+3] = p4;
            boards_area += w * h;
        }

        vector<pt> hull = convex_hull(points, false);
        ld hull_area = area(hull);

        cout << fixed;
        cout.precision(1);
        cout << (boards_area / hull_area) * 100 << " %" << '\n';
    }

    cout << flush;
}