#include <iostream>
#include <numeric>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-25
 */

/**
 * Rational arithmetic implementation
 *
 * Algorithm:
 *
 * Implements the "pen and paper" method for rational arithmetic.
 * Whenever possible, the rational number is reduced to its simplest form
 * by using the GCD between the numerator and denominator.
 *
 * Time complexity:
 *  - O(1) For operations themselves
 *  - O(log(min(n, m))) For GCD computation
 *
 * (Obviously this depends on the implementation of the GCD function,
 * however the Euclidean algorithm is the most probable implementation)
 * 
 * Space complexity:
 * O(1)
 *
 * Usage:
 *
 * Uses a 64-bit signed integer and is therefore limited to
 * 2**63 - 1 for the numerator and denominator.
 */

using namespace std;
using ll = long long;

/**
 * Rational number
 */
class RNum
{
public:

/** Numerator */
ll numer;
/** Denominator */
ll denom;

/**
 * Constructor
 *
 * @param numer Numerator
 * @param denom Denominator
 */
RNum(ll numer, ll denom) : numer(numer), denom(denom)
{
    if (denom == 0) {
        throw invalid_argument("Denominator cannot be zero");
    }

    reduce();
}

/**
 * Addition
 *
 * @param other Other num
 */
RNum operator+(const RNum & other) const
{
    ll new_numer = numer * other.denom + other.numer * denom;
    ll new_denom = denom * other.denom;
    return RNum(new_numer, new_denom);
}

/**
 * Subtraction
 *
 * @param other Other num
 */
RNum operator-(const RNum & other) const
{
    ll new_numer = numer * other.denom - other.numer * denom;
    ll new_denom = denom * other.denom;
    return RNum(new_numer, new_denom);
}

/**
 * Multiplication
 *
 * @param other Other num
 */
RNum operator*(const RNum & other) const
{
    ll new_numer = numer * other.numer;
    ll new_denom = denom * other.denom;
    return RNum(new_numer, new_denom);
}

/**
 * Division
 *
 * @param other Other num
 */
RNum operator/(const RNum & other) const
{
    if (other.numer == 0) {
        throw invalid_argument("Division by zero");
    }
    ll new_numer = numer * other.denom;
    ll new_denom = denom * other.numer;
    return RNum(new_numer, new_denom);
}

/**
 * Comparison operators
 */

bool operator==(const RNum & other) const
{ return numer * other.denom == other.numer * denom; }
bool operator!=(const RNum & other) const
{ return !(*this == other); }
bool operator<(const RNum & other) const
{ return numer * other.denom < other.numer * denom; }
bool operator<=(const RNum & other) const
{ return numer * other.denom <= other.numer * denom; }
bool operator>(const RNum & other) const
{ return numer * other.denom > other.numer * denom; }
bool operator>=(const RNum & other) const
{ return numer * other.denom >= other.numer * denom; }

private:
/**
 * Reduce the rational number to its simplest
 * form by dividing both numerator and
 * denominator by their GCD.
 */
void reduce()
{
    ll d = gcd(numer, denom);
    numer /= d;
    denom /= d;

    if (denom < 0) {
        numer = -numer;
        denom = -denom;
    }
}
};

/**
 * Print overload for RNum
 */
ostream & operator<<(ostream & os, const RNum & r)
{
    os << r.numer << " / " << r.denom;
    return os;
}

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        int x1, x2, y1, y2;
        char op;
        cin >> x1 >> y1 >> op >> x2 >> y2;

        RNum r1(x1, y1);
        RNum r2(x2, y2);

        switch (op) {
            case '+':
                cout << (r1 + r2) << endl;
                break;
            case '-':
                cout << (r1 - r2) << endl;
                break;
            case '*':
                cout << (r1 * r2) << endl;
                break;
            case '/':
                cout << (r1 / r2) << endl;
                break;
            default:
                cerr << "Unknown operation: " << op << endl;
        }
    }
}