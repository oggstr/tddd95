#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

/* 
Does not work.
*/

using namespace std;
using ll = long long;

class Vec2D;

// Alias types to make code clearer
using Point   = Vec2D;
using Polygon = vector<Point>;
using Segment = pair<Point, Point>;

const double EPS = 10e-9;
const double PI  = acos(-1.0);
const double INF = 1e18;

class Vec2D
{
public:
/**
 * X coord
 */
double x;
/**
 * Y coord
 */
double y;
/**
 * New vector
 * @param x X coord
 * @param y Y coord
 */
Vec2D(double x = 0, double y = 0) : x(x), y(y) {}
/**
 * Minus operator
 * @param other Vector
 */
Vec2D operator-(const Vec2D & other) const { return Vec2D(x - other.x, y - other.y); }
/**
 * Plus operator
 * @param other Vector
 */
Vec2D operator+(const Vec2D & other) const { return Vec2D(x + other.x, y + other.y); }
/**
 * Scalar multiplication operator
 * @param scalar Scalar value
 */
Vec2D operator*(double scalar) const { return Vec2D(x * scalar, y * scalar); }
/**
 * Scalar division operator
 * @param scalar Scalar value
 */
Vec2D operator/(double scalar) const {
    if (scalar == 0) {
        throw invalid_argument("Division by zero");
    }
    return Vec2D(x / scalar, y / scalar);
}
/**
 * Equality operator
 * @param other Vector
 */
bool operator==(const Vec2D & other) const {
    return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
}

/**
 * Cross product
 * @param other Vector
 */
double cross(const Vec2D& other) const {
    return x * other.y - y * other.x;
}

/**
 * Dot product
 * @param other Vector
 */
double dot(const Vec2D& other) const {
    return x * other.x + y * other.y;
}

/**
 * Distance to another vector
 * @param other Vector
 */
double dist(const Vec2D& other) const {
    return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}
};

/**
 * Check if point p lies on the line segment s.
 * @param p Point
 * @param s Segment
 * @return true/false
 */
bool is_on_segment(Point& p, Segment& s) {
    return (p.x >= min(s.first.x, s.second.x) - EPS &&
            p.x <= max(s.first.x, s.second.x) + EPS &&
            p.y >= min(s.first.y, s.second.y) - EPS &&
            p.y <= max(s.first.y, s.second.y) + EPS);
}

/**
 * Find intersection between two line segments
 * @param s1 First segment
 * @param s2 Second segment
 * @return Vector of intersection points (0, 1, or 2 points)
 */
vector<Point> intersect(Segment s1, Segment s2)
{
    // line 1
    Point p = s1.first;
    Vec2D r = s1.second - s1.first;

    // line 2
    Point q = s2.first;
    Vec2D s = s2.second - s2.first;

    double rXs = r.cross(s);
    Vec2D pq = q - p;

    // lines collinear if their direction vectors are parallel (rXs)
    // and vector between their starting points (pq) is parallel to any line's direction
    if (abs(rXs) < EPS && abs(pq.cross(r)) < EPS) {
        // candidate points
        vector<Point> pts;

        if (is_on_segment(s1.first,  s2)) pts.push_back(s1.first);
        if (is_on_segment(s1.second, s2)) pts.push_back(s1.second);
        if (is_on_segment(s2.first,  s1)) pts.push_back(s2.first);
        if (is_on_segment(s2.second, s1)) pts.push_back(s2.second);

        // No overlap
        if (pts.empty()) return {};

        std::sort(pts.begin(), pts.end(), [](const Point& pt1, const Point& pt2)
        {
            // Prefer X
            if (abs(pt1.x - pt2.x) > EPS) {
                return pt1.x < pt2.x;
            }

            // X are equal
            if (abs(pt1.y - pt2.y) > EPS) {
                 return pt1.y < pt2.y;
            }

            return false;
        });

        // Remove duplicate points
        pts.erase(std::unique(pts.begin(), pts.end(), [](Point pt1, Point pt2){
            return abs(pt1.x - pt2.x) < EPS && abs(pt1.y - pt2.y) < EPS;
        }), pts.end());

        // Should be impossible
        if (pts.empty()) return {};

        // Point overlap
        if (pts.size() == 1) return {pts[0]};
        
        // Segment overlap
        return {pts.front(), pts.back()};
    }

    // Lines are parallel but NOT collinear
    if (abs(rXs) < EPS) { 
        return {};
    }

    // Single point intersect
    // Solve for t and u: p + t * r = q + u * s
    // t => s1, u => s2
    double t = pq.cross(s) / rXs;
    double u = pq.cross(r) / rXs; 

    // Check intersect lies within both segments
    if (t >= 0 - EPS &&
        t <= 1 + EPS &&
        u >= 0 - EPS &&
        u <= 1 + EPS) 
    {
        Point intersection_point = p + r * t;
        return {intersection_point};
    }

    // Lines intersect, segments do not
    return {};
}

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n_cases;
    cin >> n_cases;
    for (int i = 0; i < n_cases; ++i) {
        double x1, y1, x2, y2, x3, y3, x4, y4;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

        Segment s1 = {Point(x1, y1), Point(x2, y2)};
        Segment s2 = {Point(x3, y3), Point(x4, y4)};

        vector<Point> result = intersect(s1, s2);

        if (result.empty()) {
            cout << "none" << '\n';
            continue;
        }

        // Avoid -0.0
        auto abs_if_zero = [](double val) {
            if (abs(val) < 5*1e-3) return 0.0;
            return val;
        };
        
        cout << fixed << setprecision(2);
        if (result.size() == 1) {
            cout << abs_if_zero(result[0].x) << ' ' << abs_if_zero(result[0].y) << '\n';
            continue;
        }

        cout << abs_if_zero(result[0].x) << ' ' << abs_if_zero(result[0].y) << ' '
                << abs_if_zero(result[1].x) << ' ' << abs_if_zero(result[1].y) << '\n';
    }

    cout << flush;
    return 0;
}
