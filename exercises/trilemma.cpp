/**
 * @author Oskar Arensmeier
 * @date 2025-04-28
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#define ld long double
#define EPS 1e-9
#define PI 3.14159265358979323846

using namespace std;

struct vec {
    ld x, y;

    vec() : x(0), y(0) {}
    vec(ld x, ld y) : x(x), y(y) {}

    vec operator-(const vec& other) const {
        return vec(x - other.x, y - other.y);
    }

    ld cross(const vec& other) const {
        return x * other.y - y * other.x;
    }

    ld dot(const vec& other) const {
        return x * other.x + y * other.y;
    }

    ld ang(const vec& other) const {
        return atan2(cross(other), dot(other));
    }

    ld len_sq() const {
        return x*x + y*y;
    }
};

bool is_collinear(const vec& a, const vec& b, const vec& c)
{
    vec ab = b - a;
    vec ac = c - a;
    return fabs(ab.cross(ac)) < EPS;
}

string angle_class(const vec& a, const vec& b, const vec& c)
{
    vec ab = b - a;
    vec bc = c - b;
    vec ca = a - c;

    ld ab_sq = ab.len_sq();
    ld bc_sq = bc.len_sq();
    ld ca_sq = ca.len_sq();

    // Pythagoras
    if (fabs(ab_sq + bc_sq - ca_sq) < EPS || 
        fabs(bc_sq + ca_sq - ab_sq) < EPS || 
        fabs(ca_sq + ab_sq - bc_sq) < EPS
    ) {
        return "right";
    }

    // Obtuse angle must have the opposite side
    // longer than the sum of the neighbor sides
    if (ab_sq + bc_sq < ca_sq || 
        bc_sq + ca_sq < ab_sq || 
        ca_sq + ab_sq < bc_sq
    ) {
        return "obtuse";
    }

    return "acute";
}

string side_class(const vec& a, const vec& b, const vec& c)
{
    vec ab = b - a;
    vec bc = c - b;
    vec ca = a - c;

    ld ab_sq = ab.len_sq();
    ld bc_sq = bc.len_sq();
    ld ca_sq = ca.len_sq();

    // Check for equal sides
    if (fabs(ab_sq - bc_sq) < EPS || 
        fabs(bc_sq - ca_sq) < EPS || 
        fabs(ca_sq - ab_sq) < EPS
    ) {
        return "isosceles";
    }

    return "scalene";
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    for (int t = 1; t <= N; t++) {
        ld x1, y1, x2, y2, x3, y3;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

        vec a(x1, y1);
        vec b(x2, y2);
        vec c(x3, y3);

        cout << "Case #" << t << ": ";

        if (is_collinear(a, b, c)) {
            cout << "not a triangle" << endl;
            continue;
        }

        cout << side_class(a, b, c) << " " << angle_class(a, b, c) << " triangle" << endl;
    }

    cout << flush;
    return 0;
}