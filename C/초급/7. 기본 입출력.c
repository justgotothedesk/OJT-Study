#include <stdio.h>

int main() {
    int n;
    
    while (1) {
        printf("�������� �� �ٿ� ��� ��� �ұ��? ");
        if (scanf("%d", &n) != 1 || n <= 0 || n > 9) {
            printf("�߸��� �Է��Դϴ�. 1���� 9 ������ ������ �Է��ϼ���.\n");
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
