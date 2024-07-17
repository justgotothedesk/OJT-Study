#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int n;
    printf("주사위를 몇 번 굴릴까요? ");
    scanf("%d", &n);

    int dice[6];
    for (int i = 0; i < 6; i++) {
        dice[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int random = rand()%6;
        dice[random] += 1;
    }

    printf("주사위를 %d번 굴려서 나온 결과입니다.\n", n);
    for (int i = 0; i < 6; i++) {
        printf("%d번: %d (%.2f%%)\n", i + 1, dice[i], dice[i] / (double)n * 100);
    }

    return 0;
}