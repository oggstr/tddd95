#include <iostream>
#include <vector>
#include <string>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-25
 */

/**
 * Implementation of Knuth-Morris-Pratt algorithm to find all occurrences
 * of a pattern in a text.
 *
 * reference: https://cp-algorithms.com/string/prefix-function.html
 *
 * Algorithm:
 *
 * The algorithm works by exploiting the definition of the prefix function.
 * Given i, pi[i] will be the length of the longest proper prefix of the substring s[0..i].
 * If we construct the string pat + DELIM + text, we can simply check if pi[i] is equal
 * to the length of the pattern (for i > len(pat)).
 *
 * Putting it all together, we first compute the prefix function of the pattern,
 * then continue to compute the prefix function for the full pat + DELIM + text string one step at a time.
 * At each point we check if the prefix function value equals the length of the pattern.
 * If it does, we have found an occurrence of the pattern in the text.
 *
 * Time complexity:
 * O(n + m)
 * Where n is the length of text and m is the length of the pattern.
 * 
 * Space complexity:
 * O(n + m)
 * 
 * Data structures:
 * - Vector for prefix function
 */

using namespace std;

class KnuthMorrisPratt {
public:

/**
 * Compute prefix function for given string
 *
 * @param s String to compute prefix func for
 * @return Vector function values
 */
static vector<int> prefix_function(const string & s)
{
    int n = s.size();
    vector<int> pi(n, 0);

    for (int i = 1; i < n; ++i) {
        int j = pi[i-1];

        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
    
        if (s[i] == s[j]) {
            ++j;
        }

        pi[i] = j;
    }

    return pi;
}

/**
 * Given a text and a pattern, find all indexes
 * of where the pattern occurs in the text.
 *
 * @param text Text
 * @param pat Pattern
 * @return Vector of indexes
 */
static vector<int> find_pattern_positions(const string & text, const string & pat)
{
    vector<int> pi = prefix_function(pat);

    // Resize to fit entires for pat + '#' + text
    pi.resize(pat.size() + text.size() + 1, 0);

    int n = pat.size();
    vector<int> res;
    for (int i = 0; i < text.size(); ++i) {

        // Index in combined string
        int ii = i + pat.size() + 1;
        int j = pi[ii-1];

        while (j > 0 && pat[j] != text[i]) {
            j = pi[j-1];
        }

        if (pat[j] == text[i]) {
            ++j;
        }

        pi[ii] = j;

        if (j == n) {
            // Compute start index
            res.push_back(i - n + 1);
        }
    }

    return res;
}
};

int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    string pat;
    string text;

    while (getline(cin, pat)) {
        getline(cin, text);

        auto res = KnuthMorrisPratt::find_pattern_positions(text, pat);

        for (auto i : res) {
            cout << i << ' ';
        }

        cout << '\n';
    }

    cout << flush;
    return 0;
}