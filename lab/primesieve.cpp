#include <iostream>
#include <cstring>

/**
 * @author Oskar Arensmeier
 * @date 2025-05-26
 */

/**
 * Sieve of Eratosthenes implementation using a bit array.
 * Reference: https://cp-algorithms.com/algebra/sieve-of-eratosthenes.html
 *
 * Algorithm:
 * Start by initing the array with all bits 1.
 * Then, for each number i starting from 2,
 * if i is still marked as prime, mark all multiples of i as not prime.
 * 
 * Time complexity: O(n log log n)
 *
 * Space complexity: O(n / 8) for the bit array
 */

using namespace std;
using ll = long long;

class PrimeSieve
{
public:

/**
 * Sieve size
 */
ll n;

/**
 * Number of primes
 */
ll m;

/**
 * Constructor
 * @param n Sieve size
 */
PrimeSieve(ll n) : n(n), m(bit_count())
{
    if (n < 0) {
        invalid_argument("n must not be negative");
    }

    init_sieve();
}

/**
 * Destructor
 */
~PrimeSieve() { delete[] sieve; }

/**
 * Check if i is prime
 * @param i Number to check
 * @return true if i is prime, false otherwise
 */
bool is_prime(ll i)
{
    if (i < 0 || i > n) {
        throw out_of_range("i must be in range [0, n]");
    }

    return bit_is_set(i);
}

/**
 * Get the number of primes in the sieve
 * @return Number of primes
 */
ll prime_count() const { return m; }

private:
/**
 * Bit array for sieve of Eratosthenes
 */
char * sieve;

/**
 * Initialize the sieve
 */
void init_sieve()
{
    sieve = new char[byte_count()];
    memset(sieve, 0xFF, byte_count());

    // 0 and 1 are not prime
    bit_flip(0);
    bit_flip(1);

    for (ll i = 2; i * i <= n; ++i) {
        if (bit_is_set(i)) {
            for (ll j = i * i; j <= n; j += i) {
                if (bit_is_set(j)) {
                    bit_flip(j);
                }
            }
        }
    }
}

/** Returns the number of bytes in sieve */
inline ll byte_count() { return n / 8 + 1; }
/** Returns the number of bits in sieve */
inline ll bit_count()  { return n + 1; }

/**
 * Flip bit i in sieve.
 */
inline void bit_flip(ll i)
{
    m --;
    sieve[i / 8] = sieve[i / 8] ^ (1 << (i % 8));
}

/**
 * Check if bit i in sieve is set.
 */
inline bool bit_is_set(ll i)
{
    return (sieve[i / 8] & (1 << (i % 8))) != 0;
}
};


int main()
{
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    ll n, q;
    cin >> n >> q;

    PrimeSieve ps(n);

    cout << ps.prime_count() << '\n';

    ll query;
    for (ll i = 0; i < q; ++i) {
        cin >> query;
        cout << (ps.is_prime(query) ? '1' : '0') << '\n';
    }

    cout << flush;
    return 0;
}