#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-15
 */

/**
 * Implementation of: https://cp-algorithms.com/algebra/fft.html
 *
 * Note, DFT = Discrete Fourier Transform
 *
 * Algorithm:
 *
 * Exploits the fact that
 *  DFT(A * B) = DFT(A) * DFT(B)
 * 
 * And subsequently
 *  A * B = InverseDFT(DFT(A * B)) = InverseDFT(DFT(A) * DFT(B))
 *
 * In other words, we're computing the A * B in the Fourier domain
 * then transforming it back to our original domain. This is substantially
 * faster then the naive O(n^2) approach since the Fast Fourier Transform
 * allows us to compute the DFT in O(n log n) time.
 *
 * Data structures:
 * - Vectors
 * - Complex numbers
 *
 * Time complexity:
 * - 2 * O(n log n) + O(n) + O(n log n) = O(n log n)
 */


using namespace std;

using cd = complex<double>;

const double PI = acos(-1);

/**
 * Fast Fourier Transform
 * as seen at https://cp-algorithms.com/algebra/fft.html.
 * The result is computed in-place on the input vector.
 *
 * @param a vector of complex numbers
 * @param invert if true, performs inverse DFT
 */
void fft(vector<cd> & a, bool invert)
{
    int n = a.size();
    if (n == 1) {
        return;
    }

    vector<cd> a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[2*i];
        a1[i] = a[2*i+1];
    }

    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    cd w(1), wn(cos(ang), sin(ang));

    for (int i = 0; 2 * i < n; i++) {

        a[i] = a0[i] + w * a1[i];
        a[i + n/2] = a0[i] - w * a1[i];

        if (invert) {
            a[i] /= 2;
            a[i + n/2] /= 2;
        }

        w *= wn;
    }
}

vector<int> multiply(vector<int> const& a, vector<int> const& b)
{
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());

    int n = 1;
    while (n < a.size() + b.size()) {
        n <<= 1;
    }

    fa.resize(n);
    fb.resize(n);

    // DFT(A), DFT(B) : O(n log n)
    fft(fa, false);
    fft(fb, false);

    // DFT(A) * DFT(B) (pairwise) : O(n)
    for (int i = 0; i < n; i++) {
        fa[i] *= fb[i];
    }

    // InverseDFT(DFT(A) * DFT(B)) : O(n log n)
    fft(fa, true);

    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = round(fa[i].real());
    }

    return result;
}

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int T;
    int n, m;

    cin >> T;

    cin >> n;
    n = n + 1;
    vector<int> pol1(n);
    for (int i = 0; i < n; i++) {
        cin >> pol1[i];
    }

    cin >> m;
    m = m + 1;
    vector<int> pol2(m);
    for (int i = 0; i < m; i++) {
        cin >> pol2[i];
    }

    /*
    From CP-Algorithms:

    "It should be noted, that the two polynomials should have the same degree.
    Otherwise the two result vectors of the DFT have different length.
    We can accomplish this by adding coefficients with the value $0$.
    
    And also, since the result of the product of two polynomials is a polynomial of degree  $2 (n - 1)$, 
    we have to double the degrees of each polynomial (again by padding  $0$ s).
    From a vector with  $n$  values we cannot reconstruct the desired polynomial with  $2n - 1$  coefficients."

    -- https://cp-algorithms.com/algebra/fft.html
    */

    int sz = (max(n, m) % 2 == 0)
        ? max(n, m)
        : max(n, m) + 1;

    pol1.resize(sz, 0);
    pol2.resize(sz, 0);

    vector<int> result = multiply(pol1, pol2);

    // Drop trailing zeros
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    cout << (result.size() - 1) << '\n';
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << ' ';
    }
    cout << '\n';
    cout << flush;

    return 0;
}