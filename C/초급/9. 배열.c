#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int n;
    printf("�ֻ����� �� �� �������? ");
    scanf("%d", &n);

    int dice[6];
    for (int i = 0; i < 6; i++) {
        dice[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int random = rand()%6;
        dice[random] += 1;
    }

    printf("�ֻ����� %d�� ������ ���� ����Դϴ�.\n", n);
    for (int i = 0; i < 6; i++) {
        printf("%d��: %d (%.2f%%)\n", i + 1, dice[i], dice[i] / (double)n * 100);
    }

    return 0;
}