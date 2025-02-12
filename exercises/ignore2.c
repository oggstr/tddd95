/**
 * @author Oskar Arensmeier
 * @date 2025-02-08
 */

int main()
{
    long long int n;
    long long int b7;
    long long int b10;
    long long int digit;
    long long int pos;
    while (scanf("%lld", &n) == 1) {
        b7 = 0;
        pos = 1;
        while (n > 0) {
            b7 += (n % 7) * pos;
            n /= 7;
            pos *= 10;
        }
        b10 = 0;
        pos = 1;
        while (b7 > 0) {
            digit =    b7 % 10 
                    + (b7 % 10 > 2 ? 2 : 0)
                    + (b7 % 10 > 4 ? 1 : 0);
            b10 += digit * pos;
            b7 /= 10;
            pos *= 10;
        }
        while (b10 > 0) {
            printf(
                "%c",
                48 + (
                    b10 % 10 == 6
                    ? 9
                    : b10 % 10 == 9
                    ? 6
                    : b10 % 10
                ) 
            );
            b10 /= 10;
        }
        printf("\n");
    }
}