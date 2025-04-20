/**
 * @author Oskar Arensmeier
 * @date 2025-04-20
 */

#include <iostream>
#include <string>
#include <map>
#include <queue>

using namespace std;

bool is_possible(string word)
{
    map<char, int> count;
    for (auto c : word) {
        count[c] ++;
    }

    int uneven = 0;
    for (auto it : count) {
        if (it.second % 2 == 1) {
            uneven ++;
        }
    }

    return uneven <= 1;
}

bool is_palindrome(string word)
{
    int i = 0;
    int j = word.size() - 1;

    while (i < j) {
        if (word[i] != word[j]) {
            return false;
        }
        i ++;
        j --;
    }

    return true;
}

int greedy_swap(string & word)
{
    int i = 0;
    int j = word.size() - 1;

    int swap_count = 0;
    while (i < j) {
        if (word[i] == word[j]) {
            i ++;
            j --;
            continue;
        }

        char c = word[i];

        // find first occurrence of c from right
        int k;
        for (k = j; k > i; -- k) {
            if (word[k] == c) {
                break;
            }
        }

        // Edge case: word[k] must end up in middle
        if (k == i) {
            // Move it right and continue
            // it will find its way to the middle eventually
            swap(word[k], word[k + 1]);
            swap_count ++;

            // Don't incr i and j, since we need to check them again
            continue;
        }

        // swap so that word[i] == word[j]
        for (; k < j; ++ k) {
            swap(word[k], word[k + 1]);
            swap_count ++;
        }

        i ++;
        j --;
    }

    return swap_count;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n; ++ i) {
        string word;
        cin >> word;

        if (! is_possible(word))  {
            cout << "Impossible\n";
            continue;
        }

        cout << greedy_swap(word) << '\n';
    }

    cout << flush;
}