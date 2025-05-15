#include <iostream>
#include <vector>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-15
 */

/**
 * Implementation of Gauss-Jordan elimination as specified by:
 * https://cp-algorithms.com/linear_algebra/linear-system-gauss.html
 *
 * Algorithm:
 *
 * The algorithm is quite simple. In essence, it's just an algorithmic implementation
 * of normal Gauss elimination from linear algebra:
 *
 * 1. Select a pivot element
 * 2. Swap the pivot row on to the diagonal
 *     (not strictly necessary, but helps the code)
 * 3. Perform elimination on all other rows
 * 4. Repeat until all rows are processed
 *
 * After this we go on to check if the system is consistent
 * (i.e. doesn't have something like 1 = 0) and if it has a unique solution.
 *
 * If the system is consistent, we go on to compute a solution.
 * Then we check if there are any free variables (tracked by the code through the `where` array).
 * If there are, we return there exists an infinite amount solutions.
 *
 *
 * Note:
 *
 * Gauss-Jordan is not a numerically stable algorithm. The situation can be improved by
 * good choices of the pivot element, but the problem cannot be solved completely.
 * There are different algorithms for solving linear systems that are numerically stable.
 *
 *
 * Data structures:
 * 
 * - Vectors
 *
 *
 * Time complexity:
 *
 * Our solve is dominated by an O( min(n, m) * n^2 ) loop. Since n = m for our problems,
 * the complexity is O(n^3). (n and m are the number of rows and columns in the matrix)
 *
 *
 * Space complexity:
 *
 * Space is dominated by the matrix, the right hand vector and our answer vector.
 * Thus, space complexity is O( (n+2) * m ) = O(n*m) = O(n^2) since n = m.
 */

using std::cin;
using std::cout;
using std::vector;
using std::abs;
using std::swap;

using lli = long long int;

template <typename T>
class GaussJordan
{
private:
    /**
     * Tolerance
     */
    const double EPS = 1e-9;

    /**
     * Matrix to be solved
     */
    vector<vector<T>> A;

    /**
     * Right hand side
     */
    vector<T> b;

    /**
     * Rows and cols in matrix
     */
    lli rows, cols;

public:
    /**
     * Solution type
     */
    enum class SolutionType
    {
        /**
         * Solution found
         */
        OK,

        /**
         * No solution found
         */
        NO_SOLUTION,

        /**
         * Infinite solutions found
         */
        INFINITE_SOLUTIONS
    };

    /**
     * Constructor for GaussJordan solver
     *
     * @param rows Rows in matrix
     * @param cols Columns in matrix
     */
    GaussJordan(lli rows, lli cols) : rows(rows), cols(cols)
    {
        A.resize(rows, std::vector<T>(cols));
        b.resize(rows);
    }

    /**
     * Constructor for GaussJordan solver
     *
     * @param A Matrix to be solved
     * @param b Right hand side
     */
    GaussJordan(
        vector<vector<T>> A,
        vector<T> b) : A(A), b(b)
    {
        rows = A.size();
        cols = A[0].size();
    }

    /**
     * Array access operator
     *
     * @param i Row index
     */
    vector<T> operator[](lli i)
    {
        return A[i];
    }

    SolutionType solve(vector<T> &ans)
    {
        vector<T> where(cols, -1);

        // Time: O( min(n, m) * nm ) = O(n^3) (Since n = m for square matrix)
        for (
            lli r = 0, c = 0;
            c < cols && r < rows;
            ++c
        ) {

            // Find pivot (largest element in column)
            lli pivot = r;
            for (lli i = r; i < rows; ++i) {
                if (abs(A[i][c]) > abs(A[pivot][c])) {
                    pivot = i;
                }
            }

            // Pivot too small
            if (abs(A[pivot][c]) < EPS) {
                continue;
            }

            // Swap current row with pivot row
            for (lli i = c; i < cols; ++i) {
                swap(A[pivot][i], A[r][i]);
            }
            swap(b[pivot], b[r]);

            // Update where array
            where[c] = r;

            for (lli i = 0; i < rows; ++i) {
                // Skip pivot row
                if (i == r)
                    continue;

                // Perform elimination on A
                double k = A[i][c] / A[r][c];
                for (lli j = c; j < cols; ++j) {
                    A[i][j] -= A[r][j] * k;
                }

                // Perform elimination on b
                b[i] -= b[r] * k;
            }

            ++r;
        }

        ans.assign(cols, 0);
        // Time: O(n)
        for (lli i = 0; i < cols; ++i) {
            if (where[i] == -1) {
                continue;
            }

            // Compute ans[i]
            ans[i] = b[where[i]] / A[where[i]][i];
        }

        // Time: O(n^2)
        for (lli i = 0; i < rows; ++i) {

            double sum = 0;
            for (lli j = 0; j < cols; ++j) {
                sum += A[i][j] * ans[j];
            }

            // Check that A[i] * ans = b[i]
            // If not, return no solution
            if (abs(sum - b[i]) > EPS) {
                return SolutionType::NO_SOLUTION;
            }
        }

        // Time: O(n)
        for (lli i = 0; i < cols; ++i) {
            if (where[i] == -1) {
                return SolutionType::INFINITE_SOLUTIONS;
            }
        }

        return SolutionType::OK;
    }
};

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(0);

    int n;
    while (cin >> n, n != 0) {

        vector<vector<double>> A(n, vector<double>(n));
        vector<double> b(n);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> A[i][j];
            }
        }

        for (int i = 0; i < n; ++i) {
            cin >> b[i];
        }

        GaussJordan<double> solver(A, b);

        vector<double> ans;
        auto result = solver.solve(ans);

        using SolutionType = GaussJordan<double>::SolutionType;

        switch (result) {
        case SolutionType::OK:
            for (lli i = 0; i < n; ++i) {
                cout << ans[i] << (i == (n-1) ? '\n' : ' ');
            }
            break;

        case SolutionType::NO_SOLUTION:
            cout << "inconsistent\n";
            break;

        case SolutionType::INFINITE_SOLUTIONS:
            cout << "multiple\n";
            break;
        }
    }

    cout << std::flush;
    return 0;
}