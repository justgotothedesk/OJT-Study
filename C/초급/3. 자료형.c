#include <stdio.h>

int main() {
    printf("자료형        크기       데이터값\n");

    char charVar = 'a';
    printf("char: %5lu %5c\n", sizeof(charVar), charVar);

    unsigned char ucharVar = 'a';
    printf("unsigned char: %5lu %5c\n", sizeof(ucharVar), ucharVar);

    int intVar = 0;
    printf("int: %5lu %5d\n", sizeof(intVar), intVar);

    short shortVar = 0;
    printf("short: %5lu %5d\n", sizeof(shortVar), shortVar);

    long longVar = 0;
    printf("long: %5lu %5ld\n", sizeof(longVar), longVar);

    unsigned short ushortVar = 0;
    printf("unsigned short: %5lu %5u\n", sizeof(ushortVar), ushortVar);

    unsigned int uintVar = 0;
    printf("unsigned int: %5lu %5u\n", sizeof(uintVar), uintVar);

    unsigned long ulongVar = 0;
    printf("unsigned long: %5lu %5lu\n", sizeof(ulongVar), ulongVar);

    float floatVar = 0.0f;
    printf("float: %5lu %.1f\n", sizeof(floatVar), floatVar);

    double doubleVar = 0.0;
    printf("double: %5lu %.1f\n", sizeof(doubleVar), doubleVar);

    char* charPtr = "a";
    printf("char*: %5lu %5s\n", sizeof(charPtr), charPtr);

    return 0;
}