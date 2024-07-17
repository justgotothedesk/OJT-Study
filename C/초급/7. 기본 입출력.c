#include <stdio.h>

int main() {
    int n;
    
    while (1) {
        printf("구구단을 한 줄에 몇개씩 출력 할까요? ");
        if (scanf("%d", &n) != 1 || n <= 0 || n > 9) {
            printf("잘못된 입력입니다. 1에서 9 사이의 정수를 입력하세요.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }

    for (int i = 1; i <= 9; i++) {
        for (int j = 2; j <= 9; j++) {
            printf("%d x %d = %-3d", j, i, j * i);
            if (j % n == 1 % n) {
                printf("\n");
            } else {
                printf("\t");
            }
        }
        printf("\n");
    }
    return 0;
}
