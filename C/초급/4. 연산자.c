#include <stdio.h>

int main() {
    int a = -3;
    int b = 3;

    printf("%d, %d, %d, %d, %d\n", a + b, a - b, a * b, a / b, a % b);
    
    printf("%d, %d, %d, %d\n", a++, ++a, b--, --b);

    a += b;
    printf("%d\n", a);

    a -= b;
    printf("%d\n", a);

    a *= b;
    printf("%d\n", a);

    a /= b;
    printf("%d\n", a);

    a %= b;
    printf("%d\n", a);

    printf("%d, %d\n", a && b, a || b);

    a = (a < b) ? 1 : a;
    b = (a >= b) ? b : 2;
    printf("%d, %d\n", a, b);

    return 0;
}