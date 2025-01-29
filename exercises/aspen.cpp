/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-22
 */

#include <cstdio>
#include <math.h>
#include <cstring>
#include <limits>

using namespace std;

#define DEBUG_PRINT for (int i = 0; i < N; ++i) { for (int j = 0; j < N; ++j) { printf("%lf ", dt[i][j]); } printf("\n"); }

int N;
double L;
double W;
double step;

int costs [2000][2000] = {0};
double dt    [2000][2000] = {0};

bool blocked_rows [2000] = {false};
bool blocked_cols [2000] = {false};

bool zero_ish(double val)
{
    return val < 0.0001;
}

double dist(double y, int target)
{
    double target_x = (target % 2) * W;
    double target_y = step * (double) (target / 2);

    return sqrt(
        pow(target_x, 2)
      + pow(y - target_y, 2)
    );
}

double find_min_row(int r)
{
    double m = dt[r][0];
    for (int c = 0; c < N; ++c) {
        if (dt[r][c] < m)
            m = dt[r][c];
    }

    return m;
}

double find_min_col(int c)
{
    double m = dt[0][c];
    for (int r = 0; r < N; ++r) {
        if (dt[r][c] < m)
            m = dt[r][c];
    }

    return m;
}

int min_lines()
{
    int dx [2000];
    int dy [2000];
    memset(dx, 0, sizeof(dx));
    memset(dy, 0, sizeof(dy));

    int total = 0;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (zero_ish(dt[r][c])) {
                dx[c] += 1;
                dy[r] += 1;
                total += 1;
            }
        }
    }

    memset(blocked_rows, false, sizeof(blocked_rows));
    memset(blocked_cols, false, sizeof(blocked_cols));

    int lines = 0;
    while (total > 0) {
        int max_row = 0;
        for (int r = 0; r < N; ++r) {
            if (dy[max_row] > dy[r]) {
                max_row = r;
            }
        }

        int max_col = 0;
        for (int c = 0; c < N; ++c) {
            if (dx[max_col] > dx[c]) {
                max_col = c;
            }
        }


        if (dy[max_row] > dx[max_col]) {
            total -= dy[max_row];
            lines ++;

            blocked_rows[max_row] = true;

            for (int c = 0; c < N; ++c) {
                if (zero_ish(dt[max_row][c])) {
                    dx[c] -= 1;
                }
            }

        } else {
            total -= dx[max_col];
            lines ++;

            blocked_cols[max_col] = true;

            for (int r = 0; r < N; ++r) {
                if (zero_ish(dt[r][max_col])) {
                    dy[r] -= 1;
                }
            }

        }
    }

    return lines;
}

double hungarian()
{
    // row subtractions
    for (int r = 0; r < N; ++r) {
        double m = find_min_row(r);
        for (int c = 0; c < N; ++c) {
            dt[r][c] -= m;
        }
    }

    // col subtractions
    for (int c = 0; c < N; ++c) {
        double m = find_min_col(c);
        for (int r = 0; r < N; ++r) {
            dt[r][c] -= m;
        }
    }

    DEBUG_PRINT;

    while (min_lines() < N) {

        double min_val = numeric_limits<double>::max();
        for (int r = 0; r < N; ++r) {
            if (blocked_rows[r])
                continue;

            for (int c = 0; c < N; ++c) {

                if (blocked_cols[c])
                    continue;

                if (dt[r][c] < min_val) {
                    min_val = dt[r][c];
                }
            }
        }
        for (int r = 0; r < N; ++r) {
            if (blocked_rows[r])
                continue;

            for (int c = 0; c < N; ++c) {

                if (blocked_cols[c])
                    continue;

                if (dt[r][c] > 0.0) {
                    dt[r][c] -= min_val;
                }
            }
        }
    }

    double res = 0.0;
    int vals_found = 0;
    bool col_picked [2000];
    memset(col_picked, false, sizeof(col_picked));

    DEBUG_PRINT;

    while (vals_found < N) {
        for (int r = 0; r < N; ++r) {
            int candidates = 0;
            int candidate_r = 0;
            int candidate_c = 0;
            for (int c = 0; c < N; ++c) {
                if (col_picked[c])
                    continue;

                if (zero_ish(dt[r][c])) {
                    candidates ++;
                    candidate_r = r;
                    candidate_c = c;
                }
            }
            if (candidates == 1) {
                res += dt[candidate_r][candidate_c];
                col_picked[candidate_c] = true;
                vals_found ++;
            }
        }
    }

    return res;
}

// NOTE: does not work
int main()
{
    scanf("%d", &N);
    scanf("%lf %lf", &L, &W);

    step = L / (N/2 - 1);


    for (int i = 0; i < N; ++i) {
        double y;
        scanf("%lf", &y);

        for (int j = 0; j < N; ++j) {
            double d = dist(y, j);
            printf("%lf\n", d);
            costs[i][j] = d;
            dt[i][j] = d;
        }
    }

    printf("%lf", hungarian());

    return 0;
}