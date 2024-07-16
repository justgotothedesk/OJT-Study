#include <stdio.h>

int main() {
    FILE* f = fopen("Hello World!.txt", "w");
    if (f == NULL) {
        perror("Error opening file for writing");
        return 0;
    }

    puts("Hello world!");
    fputs("Hello world!", f);
    fclose(f);

    f = fopen("Hello World!.txt", "a");
    if (f == NULL) {
        perror("Error opening file for reading");
        return 1;
    }

    printf("Hello World!\n");
    fprintf(f, "%s\n", "Hello World!");
    fclose(f);

    return 0;
}
