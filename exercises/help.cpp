/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-22
 */

#include <string>
#include <vector>
#include <cstdio>

using namespace std;

typedef unsigned uint;

#define BUF_SIZE 10000000

char buf [BUF_SIZE] = {0};

vector<string> split_str(const string & s, char delim)
{
    vector<string> tokens;
    string token;
    for (char c : s) {
        if (c == delim) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

string join_str(const vector<string> & v)
{
    string s;
    for (const string & w : v) {
        s += w;
        s += " ";
    }

    return s;
}

bool is_pat(const string & w)
{
    return w[0] == '<';
}

void replace_all(
    vector<string> & v,
    const string from,
    const string to
) {
    for (uint i = 0; i < v.size(); ++i) {
        if (v[i] == from) {
            v[i] = to;
        }
    }
}

bool greedy_replace(
    vector<string> & a,
    vector<string> & b
) {
    bool changed = false;
    for (uint i = 0; i < a.size(); ++i) {
        const string & w1 = a[i];
        const string & w2 = b[i];

        if (is_pat(w1) && ! is_pat(w2)) {
            replace_all(a, w1, w2);
            changed = true;
        } else if (! is_pat(w1) && is_pat(w2)) {
            replace_all(b, w2, w1);
            changed = true;
        }
    }

    return changed;
}

string solve(
    vector<string> & a,
    vector<string> & b
) {
    while (greedy_replace(a, b)) {}

    // Fix any patterns that could not be determined
    for (uint i = 0; i < a.size(); ++i) {
        const string & w1 = a[i];
        const string & w2 = b[i];

        if (is_pat(w1)) {
            replace_all(a, w1, "wasd");
        }

        if (is_pat(w2)) {
            replace_all(b, w2, "wasd");
        }
    }

    string sol_a = join_str(a);
    string sol_b = join_str(b);

    if (sol_a == sol_b)
        return sol_a;

    return "-";
}

int main()
{
    uint N;
    char cases_str [100];

    fgets(cases_str, 10, stdin);
    sscanf(cases_str, "%u", &N);

    string line;

    vector<string> a;
    vector<string> b;

    for (;N != 0; --N) {
        fgets(buf, BUF_SIZE, stdin);
        line = string(buf);
        a = split_str(line.substr(0, line.length()-1), ' '); // Split and remove trailing newline

        fgets(buf, BUF_SIZE, stdin);
        line = string(buf);
        b = split_str(line.substr(0, line.length()-1), ' '); // Split and remove trailing newline

        if (a.size() != b.size()) {
            printf("-\n");
            continue;
        }

        printf("%s\n", solve(a, b).c_str());
    }

    return 0;
}