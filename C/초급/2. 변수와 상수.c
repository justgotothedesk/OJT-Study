#include <stdio.h>

int main() {
    int a, b, c;
    a = -100;
    b = 0;
    c = 50;

    float aa, bb, cc;
    aa = 3.1;
    bb = -3.1;
    cc = 3.0;

    char aaa, bbb, ccc;
    aaa = 'a';
    bbb = 'A';
    ccc = '?';

    printf("int type: %d, %d, %d\n", a, b, c);
    printf("float type: %.1f, %.1f, %.1f\n", aa, bb, cc);
    printf("char type: %c, %c, %c\n", aaa, bbb, ccc);

    return 0;
}