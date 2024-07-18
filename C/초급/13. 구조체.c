#include <stdio.h>

struct student {
    char name[256];
    int id, kor, eng, math, sci;
};

int main() {
    int n;
    printf("������ �л��� �� ���ΰ���? ");
    scanf("%d", &n);

    struct student s[n];

    for (int i = 0; i < n; i++) {
        printf("%d�� �л��� �й��� �Է����ּ���: ", i+1);
        scanf("%d", &s[i].id);
        printf("%d�� �л��� �̸��� �Է����ּ���: ", i+1);
        scanf("%s", s[i].name);
        printf("%d�� �л��� ���� ������ �Է����ּ���: ", i+1);
        scanf("%d", &s[i].kor);
        printf("%d�� �л��� ���� ������ �Է����ּ���: ", i+1);
        scanf("%d", &s[i].eng);
        printf("%d�� �л��� ���� ������ �Է����ּ���: ", i+1);
        scanf("%d", &s[i].math);
        printf("%d�� �л��� ���� ������ �Է����ּ���: ", i+1);
        scanf("%d", &s[i].sci);
    }


    while (1) {
        printf("�л� ���� ���� �ý����Դϴ�.\n");
        printf("1. �л� ���� ��ȸ\n");
        printf("2. �л� ���� ����\n");
        printf("������ ���� �帱���? ");
        int num;
        scanf("%d", &num);

        printf("���� ��ϵ� �л� ���\n");
        for (int i = 0; i < n; i++) {
            printf("%d. %s\n", i+1, s[i].name);
        }

        if (num == 1) {
            printf("� �л��� ������ ��ȸ �Ͻðڽ��ϱ�? ");
            int target;
            scanf("%d", &target);
            target -= 1;

            printf("1. ��ü ���� ��ȸ\n");
            printf("2. ��ü ���� ���\n");
            printf("3. ����\n");
            printf("������ ���͵帱���? ");
            int stu;
            scanf("%d", &stu);

            if (stu == 1) {
                printf("�̸�: %s, �й�: %d, ����: %d, ����: %d, ����: %d, ����: %d\n",
                    s[target].name, s[target].id, s[target].kor, s[target].eng, s[target].math, s[target].sci);
            } else if (stu == 2) {
                float average = (s[target].eng + s[target].kor + s[target].math + s[target].sci) / 4.0;
                printf("%s�� ��� ������ %.2f���Դϴ�.\n", s[target].name, average);
            }
        } else if (num == 2) {
            printf("� �л��� ������ �����Ͻðڽ��ϱ�? ");
            int target;
            scanf("%d", &target);
            target -= 1;

            printf("%d�� �л��� �й��� �Է����ּ���: ", target+1);
            scanf("%d", &s[target].id);
            printf("%d�� �л��� �̸��� �Է����ּ���: ", target+1);
            scanf("%s", s[target].name);
            printf("%d�� �л��� ���� ������ �Է����ּ���: ", target+1);
            scanf("%d", &s[target].kor);
            printf("%d�� �л��� ���� ������ �Է����ּ���: ", target+1);
            scanf("%d", &s[target].eng);
            printf("%d�� �л��� ���� ������ �Է����ּ���: ", target+1);
            scanf("%d", &s[target].math);
            printf("%d�� �л��� ���� ������ �Է����ּ���: ", target+1);
            scanf("%d", &s[target].sci);  
        } else {
            printf("���α׷��� �����մϴ�.");
            break;
        }
    }

    return 0;
}
