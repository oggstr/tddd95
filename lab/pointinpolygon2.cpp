#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-28
 */

/**
 * Implementation of crossing number algorithm
 *
 * Algorithm:
 * Ref: https://en.wikipedia.org/wiki/Point_in_polygon
 *
 * The algorithm is quite simple:
 * Loop over all pairs of adjacent points in the polygon.
 * For each pair, check if the edge they form intersects with
 * and infinite line extending to the given point.
 * If it does, toggle the inside/outside state (simpler than count intersections).
 *
 * Time complexity: O(n)
 * Space complexity: O(n)
 */

using namespace std;
using ll = long long;

class Vec2D;

// Alias types to make code clearer
using Point   = Vec2D;
using Polygon = vector<Point>;

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

/**
 * Angle between this vector and another vector
 * @param other Vector
 */
double angle(const Vec2D& other) const {
    double dotProduct = dot(other);
    double magnitudeA = sqrt(x * x + y * y);
    double magnitudeB = sqrt(other.x * other.x + other.y * other.y);
    return acos(dotProduct / (magnitudeA * magnitudeB));
}

/**
 * Normalize the vector
 * @return Normalized vector
 */
Vec2D norm()
{
    double magnitude = sqrt(x * x + y * y);
    if (magnitude < EPS) {
        throw invalid_argument("Cannot normalize a zero vector");
    }
    return Vec2D(x / magnitude, y / magnitude);
}
};


int point_in_polygon(const Point& p, Polygon& poly)
{
    bool inside = false;

    for (int i = 0, j = poly.size() - 1; i < poly.size(); j = i++) {
        const Point& p1 = poly[i];
        const Point& p2 = poly[j];

        // Check if point is on the segment
        if (abs((p2 - p1).cross(p - p1)) < EPS && 
            (p.x - p1.x) * (p.x - p2.x) <= 0 && 
            (p.y - p1.y) * (p.y - p2.y) <= 0) {
            return 0;
        }

        // Check if ray intersects with edge
        if ((p1.y > p.y) != (p2.y > p.y)) {

            double x_intersect = p1.x + (p2.x - p1.x) * (p.y - p1.y) / (p2.y - p1.y);
            if (x_intersect < p.x) {
                inside = !inside;
            }
        }
    }

    return inside ? 1 : -1;
}

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n, m;
    while (1) {
        cin >> n;
        if (n == 0) {
            break;
        }

        Polygon poly;
        for (int i = 0; i < n; ++i) {
            double x, y;
            cin >> x >> y;
            poly.push_back({x, y});
        }

        cin >> m;
        for (int i = 0; i < m; ++i) {
            double x, y;
            cin >> x >> y;
            Point p(x, y);

            int res = point_in_polygon(p, poly);

            if (res == 1) {
                cout << "in\n";
            } else if (res == -1) {
                cout << "out\n";
            } else {
                cout << "on\n";
            }
        }
    }

    cout << flush;
    return 0;
}