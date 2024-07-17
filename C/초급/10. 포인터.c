#include <stdio.h>

int main() {
    int row, col;
    printf("배열의 크기를 입력하세요: ");
    scanf("%d %d", &row, &col);

    char arr[row][col];

    for (int i = 0; i < row * col; i++) {
        *((char*)arr + i) = (char)(i + 97);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }

    int target;
    printf("몇 번째 값을 알려드릴까요? ");
    scanf("%d", &target);

    printf("%d번째 값은 [%c]입니다.\n", target, (char)(*((char*)arr + target) - 33));

    return 0;
}