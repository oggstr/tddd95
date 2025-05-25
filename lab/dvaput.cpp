#include <iostream>
#include <vector>
#include <string>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-25
 */

/**
 * Extended suffix array implementation to compute LCP array (Kasai's algorithm).
 * Reference: https://cp-algorithms.com/string/suffix-array.html
 *
 * Algorithm:
 * The only difference to the basic suffix array is that
 * the LCP array is computed after the suffix array is built.
 *
 * This works by using the inverse suffix array (rank) to find
 * the longest common prefix between consecutive suffixes. It exploits
 * the fact that suffixes are sorted and lexicographically adjacent
 * suffixes share a common prefixes.
 *
 * Time complexity:
 * O(n) for the LCP array!
 *
 * Space complexity:
 * O(n) for the LCP array
 *
 * Usage:
 * Only supporting ASCII characters!
 */

using namespace std;

class SuffixArray {

static const int ALPHABET_SIZE = 256; // ASCII size

public:

/**
 * Suffix array
 */
vector<int> suffix;

/**
 * Lcp array
 */
vector<int> lcp;

/**
 * Constructor
 *
 * @param str String
 */
SuffixArray(const string & str)
{
    // Append unique char
    this->str = str + (char) 0x1;
    this->suffix = sort_cyclic_shifts(this->str);
    this->str = this->str.substr(0, this->str.size() - 1);

    // Remove '0x1'
    this->suffix.erase(this->suffix.begin());

    build_lcp();
}

/**
 * Get index of i-th suffix
 *
 * @param i i-th suffix
 * @return suffix index
 */
int get_suffix(int i) const
{
    if (i < 0 || i >= suffix.size()) {
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
    vector<int> perm(n);                       // Permutation of indices
    vector<int> cls(n);                        // Equivalence classes
    vector<int> cnt(max(ALPHABET_SIZE, n), 0); // Count of chars - for count sort

    /* 
    Inital phase sorts based on single char,
    and creates first equivalence classes.
    */

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

    /* 
    Second phase continuously increases the length of the cyclic shifts
    being sorted, doubling the length each iteration.
    */

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

        // Radix like idea using the previous equivalence classes
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

/**
 * Build LCP array
 *
 * @return LCP array
 */
void build_lcp()
{
    int n = str.size();

    // Inverse suffix arr
    vector<int> rank(n, 0);
    for (int i = 0; i < n; i++) {
        rank[suffix[i]] = i;
    }

    int k = 0;
    lcp.resize(n - 1, 0);
    for (int i = 0; i < n; i++) {

        // Largest suffix has no successor
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }

        // Get suffix after i
        int j = suffix[rank[i] + 1];

        // Check if we can improve LCP for this pair
        while (i + k < n && j + k < n && str[i+k] == str[j+k]) {
            k++;
        }

        // Store the computed LCP.
        lcp[rank[i]] = k;

        // If k > 0, lcp for next pair is at least k-1.
        if (k) {
            k--;
        }
    }
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

    getline(cin, str);
    getline(cin, str);

    SuffixArray sa(str);
    cout << *max_element(sa.lcp.begin(), sa.lcp.end()) << '\n';

    return 0;
}
