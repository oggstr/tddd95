#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-28
 */

using namespace std;
using ll = long long;

class Vec2D;

// Alias types to make code clearer
using Point   = Vec2D;
using Polygon = vector<Point>;
using Segment = pair<Point, Point>;

const double EPS = 10e-12;
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
 * Assignment operator
 */
Vec2D& operator=(const Vec2D & other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
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

vector<Point> intersect_col(Segment s1, Segment s2)
{
    Vec2D u = s1.first - s2.first;
    Vec2D v = s1.second - s1.first;

    // Parallel
    if (abs(v.cross(u)) > EPS) {
        return {};
    }

    auto in_between = [](int a, int b, int x) 
    {
        return (a <= x && x <= b) || (b <= x && x <= a);
    };

    // Check for overlap
    if (! in_between(s1.first.x, s1.second.x, s2.first.x) &&
        ! in_between(s1.first.x, s1.second.x, s2.second.x) &&
        ! in_between(s2.first.x, s2.second.x, s1.first.x) &&
        ! in_between(s2.first.x, s2.second.x, s1.second.x)) {
        return {};
    }

    vector<Point> result {s1.first, s1.second, s2.first, s2.second};
    sort(result.begin(), result.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    // Leave only overlap points
    result.erase(result.begin());
    result.pop_back();

    // Overlap is a single point
    if (result[0].dist(result[1]) < EPS) {
        result.pop_back();
    }

    return result;
}

vector<Point> intersect(Segment s1, Segment s2)
{
    Vec2D p = s1.first;
    Vec2D r = s1.second - s1.first;

    Vec2D q = s2.first;
    Vec2D s = s2.second - s2.first;

    // Segments are parallel/collinear
    double rXs = r.cross(s);
    if (abs(rXs) < EPS) {
        return intersect_col(s1, s2);
    }

    Vec2D pq = q - p;
    double t = pq.cross(s) / rXs;
    double u = pq.cross(r) / rXs;

    // Intersection point outside segments
    if (t < 0 || t > 1 || u < 0 || u > 1) {
        return {};
    }

    Point res = p + r * t;
    return {res};
}

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int x1, x2, x3, x4;
        int y1, y2, y3, y4;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

        Segment s1 = {Point(x1, y1), Point(x2, y2)};
        Segment s2 = {Point(x3, y3), Point(x4, y4)};

        vector<Point> result = intersect(s1, s2);
        if (result.empty()) {
            cout << "none" << '\n';
            continue;
        }

        cout.precision(2);
        cout << fixed;
        if (result.size() == 1) {
            cout << result[0].x << ' ' << result[0].y << '\n';
        } else {
            cout << result[0].x << ' ' << result[0].y << ' '
                 << result[1].x << ' ' << result[1].y << '\n';
        }
    }

    cout << flush;
    return 0;
}