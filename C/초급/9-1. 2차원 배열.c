#include <stdio.h>

int main() {
    int n;
    printf("출력할 행렬의 크기를 입력 하세요: ");
    scanf("%d", &n);

    int arr[n][n];
    int value = 1;
    int top = 0, bottom = n - 1, left = 0, right = n - 1;

    while (value <= n*n) {
        for (int i = top; i <= bottom; i++) {
            arr[i][left] = value++;
        }
        left++;

        for (int i = left; i <= right; i++) {
            arr[bottom][i] = value++;
        }
        bottom--;

        for (int i = bottom; i >= top; i--) {
            arr[i][right] = value++;
        }
        right--;

        for (int i = right; i >= left; i--) {
            arr[top][i] = value++;
        }
        top++;
    }

    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", arr[i][j]);
        }
        printf("\n");
    }

    return 0;
}
