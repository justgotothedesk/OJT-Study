#include <stdio.h>

int main() {
    int alpha = 65;
    int dot = 4;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if ((j == dot) || (8-j == dot)) {
                printf("%c ", alpha);
            }
            else {
                printf(". ");
            }
        }
        printf("\n");

        if (i < 4) {
            alpha += 1;
            dot -= 1;
        } else {
            alpha -= 1;
            dot += 1;
        }
    }

    return 0;
}
