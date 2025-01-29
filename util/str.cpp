/**
 * @author: Oskar Arensmeier
 * @date: 2025-01-22
 */

#include <string>
#include <vector>

using namespace std;

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

// Join a vector of strings into one string
string join_str(const vector<string> & v, char delim)
{
    string s;
    for (const string & w : v) {
        s += w;
        s += delim;
    }

    if (s.size() > 1) {
        s = s.substr(0, s.size()-1); 
    }

    return s;
}
