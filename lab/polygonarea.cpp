#include <iostream>
#include <vector>
#include <cmath>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-27
 */

/**
 * Implementation of a Vec2D class and area calculation for polygons.
 *
 * Algorithm:
 * Reference: https://cp-algorithms.com/geometry/area-of-simple-polygon.html
 * 
 * Works by adding up signed areas of trapezoids formed by polygon edges.
 * 
 * Time complexity: O(n)
 * Space complexity: O(n)
 * Usage: Assumes that the polygon is simple (no self-intersections).
 */

using namespace std;
using ll = long long;

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
Vec2D operator-(const Vec2D& other) const { return Vec2D(x - other.x, y - other.y); }
/**
 * Plus operator
 * @param other Vector
 */
Vec2D operator+(const Vec2D& other) const { return Vec2D(x + other.x, y + other.y); }
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
};

// Alias types to make code clearer
using Point = Vec2D;
using Polygon = vector<Point>;

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

int main()
{
    int n;
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

        auto [a, is_cw] = area(poly);
        cout << fixed;
        cout.precision(1);
        cout << (is_cw ? "CW" : "CCW") << " " << a << endl;
    }
}