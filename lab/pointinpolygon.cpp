#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;
using ll = long long;

class Vec2D;

// Alias types to make code clearer
using Point = Vec2D;
using Polygon = vector<Point>;

const double EPS = 10e-9;
const double PI = acos(-1.0);

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

/**
 * Swap overload for Vec2D
 * @param a First vector
 * @param b Second vector
 */
void swap(const Vec2D& a, const Vec2D& b) {
    swap(a.x, b.x);
    swap(a.y, b.y);
}

/**
 * Compute polygon area and orientation
 * @param poly Polygon
 * @return {area, is_cw}
 */
pair<double, bool> area(const Polygon& poly) {
    double area = 0.0;
    int n = poly.size();

    for (int i = 0; i < n; ++i) {
        const Point& p1 = i ? poly[i-1] : poly.back();
        const Point& p2 = poly[i];
        area += (p1.x - p2.x) * (p1.y + p2.y);
    }

    bool is_cw = area < 0;
    return {abs(area) / 2.0, is_cw};
}

/**
 * Check if p is one the line segment
 * defined by p1 and p2
 * @param p Point to check
 * @param p1 First point of segment
 * @param p2 Second point of segment
 * @return true/false
 */
bool is_on_line(const Vec2D& p, const Vec2D& p1, const Vec2D& p2)
{
    Vec2D edge = p2 - p1;
    return abs(edge.cross(p - p1)) < EPS &&     // Point is on the line defined by edge
           (p.x - p1.x) * (p.x - p2.x) <= 0 &&  // Point is between p1 and p2 x-axis
           (p.y - p1.y) * (p.y - p2.y) <= 0;    // Point is between p1 and p2 y-axis
}

int point_in_polygon(const Point& p, Polygon& poly)
{
    // Empty polygon
    if (poly.empty()) {
        return -1;
    }

    // Single point polygon
    if (poly.size() == 1) {
        return p.dist(poly[0]) < EPS ? 0 : -1;
    }

    // Two point polygon
    if (poly.size() == 2) {
        Point & p1 = poly[0];
        Point & p2 = poly[1];
        return is_on_line(p, p1, p2) ? 0 : -1;
    }

    // Winding number
    int wn = 0;

    for (int i = 0; i < poly.size(); ++i) {
        Point & p1 = poly[i];
        Point & p2 = poly[(i + 1) % poly.size()];
        Vec2D edge = p2 - p1;

        if (is_on_line(p, p1, p2)) {
            return 0;
        }

        /*
        Upward crossing

        p1.y < p.y < p2.y       : p's x-axis is crossed
        edge.cross(p - p1) > 0  : p is on the left of edge (in edge's direction)
        */
        if (p1.y < p.y) {
            if (p2.y > p.y && edge.cross(p - p1) > EPS) {
                // Upward crossing
                ++wn;
            }

        /* 
        Downward crossing

        p2.y <= p.y && p1.y < p.y : p's x-axis is crossed
        edge.cross(p - p1) < 0    : p is on the right of edge (in edge's direction)
        */
        } else {
            if (p2.y <= p.y && edge.cross(p - p1) < EPS) {
                // Downward crossing
                --wn;
            }
        }
    }

    // If winding number is non-zero, point is inside
    return (wn != 0) ? 1 : -1;
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