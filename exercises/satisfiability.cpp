/**
 * @author Oskar Arensmeier
 * @date 2024-04-21
 */

#include <iostream>
#include <limits>
#include <cstring>
#include <string>

#define llu long long unsigned

using namespace std;

// State of variables
llu state;

// State of negated variables
llu state_neg;

// Positive part of clauses (X1 v X2 v ...)
// Each bit represents a variable
llu clauses[101];

// Negative part of clauses (~X1 v ~X2 v ...)
// Each bit represents a variable
llu clauses_neg[101];

int N, M;

bool satisfiable()
{
    for (llu s = 0; s < (1LLU << N); ++ s) {
        llu state = s;
        llu state_neg = ~state;

        bool satisfied = true;
        for (int i = 0; i < M; ++ i) {
            llu res = clauses[i] & state;
            llu res_neg = clauses_neg[i] & state_neg;

            if (res == 0 && res_neg == 0) {
                satisfied = false;
                break;
            }
        }

        if (satisfied) {
            return true;
        }
    }

    return false;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    for (int t = 0; t < T; ++ t) {
        memset(clauses, 0LLU, sizeof(clauses));
        memset(clauses_neg, 0LLU, sizeof(clauses_neg));

        cin >> N >> M;
        cin.ignore();

        for (int i = 0; i < M; ++ i) {
            string clause;
            getline(cin, clause);

            // Split on ' v '
            size_t pos = 0;
            while (! clause.empty()) {

                pos = clause.find(" v ");
                if (pos == string::npos) {
                    pos = clause.length();
                }

                string token = clause.substr(0, pos);

                if (pos == clause.length()) {
                    clause.clear();
                } else {
                    clause.erase(0, pos + 3);
                }

                // Check negation
                if (token[0] == '~') {
                    // Negation
                    int var = stoi(token.substr(2)) - 1;
                    clauses_neg[i] |= (1LLU << var);
                } else {
                    // Positive
                    int var = stoi(token.substr(1)) - 1;
                    clauses[i] |= (1LLU << var);
                }
            }
        }

        if ( satisfiable() ) {
            cout << "satisfiable\n";
        } else {
            cout << "unsatisfiable\n";
        }
    }

    cout << flush;
}