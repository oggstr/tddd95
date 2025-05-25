#include <iostream>
#include <vector>
#include <string>

using namespace std;

class SuffixArray {

static const int ALPHABET_SIZE = 256; // ASCII size

public:

/**
 * Suffix array
 */
vector<int> suffix;

SuffixArray(const string & str)
{
    // Append unique char
    this->str = str + (char) 0x1;
    this->suffix = sort_cyclic_shifts(this->str);

    // Remove '$'
    this->suffix.erase(this->suffix.begin());
}

int get_suffix(int i) const
{
    if (i < 0 || i >= str.size()-1) {
        throw out_of_range("Index out of range");
    }

    return suffix[i];
}

private:
/**
 * String
 */
string str;

/**
 * Sort cyclic shifts of the string
 *
 * @param s String
 * @return Sorted suffixes
 */
vector<int> sort_cyclic_shifts(string const& s) {
    int n = s.size();
    vector<int> perm(n);               // Permutation of indices
    vector<int> cls(n);                // Equivalence classes
    vector<int> cnt(ALPHABET_SIZE, 0); // Count of chars - for count sort

    for (int i = 0; i < n; i++) {
        cnt[s[i]]++;
    }
    for (int i = 1; i < ALPHABET_SIZE; i++) {
        cnt[i] += cnt[i-1];
    }
    for (int i = 0; i < n; i++) {
        perm[--cnt[s[i]]] = i;
    }

    cls[perm[0]] = 0;
    int cls_count = 1;
    for (int i = 1; i < n; i++) {
        if (s[perm[i]] != s[perm[i-1]]) {
            cls_count++;
        }

        cls[perm[i]] = cls_count - 1;
    }

    vector<int> perm_tmp(n);
    vector<int> cls_tmp(n);
    for (int h = 0; (1 << h) < n; ++h) {

        for (int i = 0; i < n; i++) {
            perm_tmp[i] = perm[i] - (1 << h);

            if (perm_tmp[i] < 0) {
                perm_tmp[i] += n;
            }
        }

        fill(cnt.begin(), cnt.begin() + cls_count, 0);

        for (int i = 0; i < n; i++) {
            cnt[cls[perm_tmp[i]]]++;
        }
        for (int i = 1; i < cls_count; i++) {
            cnt[i] += cnt[i-1];
        }
        for (int i = n-1; i >= 0; i--) {
            perm[--cnt[cls[perm_tmp[i]]]] = perm_tmp[i];
        }

        cls_tmp[perm[0]] = 0;
        cls_count = 1;
        for (int i = 1; i < n; i++) {

            pair<int, int> cur  = {cls[perm[i]],   cls[(perm[i]   + (1 << h)) % n]};
            pair<int, int> prev = {cls[perm[i-1]], cls[(perm[i-1] + (1 << h)) % n]};

            if (cur != prev) {
                ++cls_count;
            }

            cls_tmp[perm[i]] = cls_count - 1;
        }

        cls.swap(cls_tmp);
    }

    return perm;
}
};

// Split string across a delimiter
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

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    string str;
    string queries_line;

    while (getline(cin, str)) {
        if (str.empty()) {
            break;
        }

        //str += '\n';

        SuffixArray sa(str);

        getline(cin, queries_line);
        vector<string> query_tokens = split_str(queries_line, ' ');

        for (size_t k = 1; k < query_tokens.size(); ++k) {
            int query_idx = stoi(query_tokens[k]);
            cout << sa.get_suffix(query_idx);

            cout << (k < query_tokens.size() - 1 ? ' ' : '\n');
        }
    }

    return 0;
}