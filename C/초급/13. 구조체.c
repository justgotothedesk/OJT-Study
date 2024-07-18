#include <stdio.h>

struct student {
    char name[256];
    int id, kor, eng, math, sci;
};

int main() {
    int n;
    printf("관리할 학생은 몇 명인가요? ");
    scanf("%d", &n);

    struct student s[n];

    for (int i = 0; i < n; i++) {
        printf("%d번 학생의 학번을 입력해주세요: ", i+1);
        scanf("%d", &s[i].id);
        printf("%d번 학생의 이름을 입력해주세요: ", i+1);
        scanf("%s", s[i].name);
        printf("%d번 학생의 국어 성적을 입력해주세요: ", i+1);
        scanf("%d", &s[i].kor);
        printf("%d번 학생의 영어 성적을 입력해주세요: ", i+1);
        scanf("%d", &s[i].eng);
        printf("%d번 학생의 수학 성적을 입력해주세요: ", i+1);
        scanf("%d", &s[i].math);
        printf("%d번 학생의 과학 성적을 입력해주세요: ", i+1);
        scanf("%d", &s[i].sci);
    }


    while (1) {
        printf("학생 정보 관리 시스템입니다.\n");
        printf("1. 학생 정보 조회\n");
        printf("2. 학생 정보 수정\n");
        printf("무엇을 도와 드릴까요? ");
        int num;
        scanf("%d", &num);

        printf("현재 등록된 학생 목록\n");
        for (int i = 0; i < n; i++) {
            printf("%d. %s\n", i+1, s[i].name);
        }

        if (num == 1) {
            printf("어떤 학생의 정보를 조회 하시겠습니까? ");
            int target;
            scanf("%d", &target);
            target -= 1;

            printf("1. 전체 정보 조회\n");
            printf("2. 전체 성적 평균\n");
            printf("3. 종료\n");
            printf("무엇을 도와드릴까요? ");
            int stu;
            scanf("%d", &stu);

            if (stu == 1) {
                printf("이름: %s, 학번: %d, 국어: %d, 영어: %d, 수학: %d, 과학: %d\n",
                    s[target].name, s[target].id, s[target].kor, s[target].eng, s[target].math, s[target].sci);
            } else if (stu == 2) {
                float average = (s[target].eng + s[target].kor + s[target].math + s[target].sci) / 4.0;
                printf("%s의 평균 성적은 %.2f점입니다.\n", s[target].name, average);
            }
        } else if (num == 2) {
            printf("어떤 학생의 정보를 수정하시겠습니까? ");
            int target;
            scanf("%d", &target);
            target -= 1;

            printf("%d번 학생의 학번을 입력해주세요: ", target+1);
            scanf("%d", &s[target].id);
            printf("%d번 학생의 이름을 입력해주세요: ", target+1);
            scanf("%s", s[target].name);
            printf("%d번 학생의 국어 성적을 입력해주세요: ", target+1);
            scanf("%d", &s[target].kor);
            printf("%d번 학생의 영어 성적을 입력해주세요: ", target+1);
            scanf("%d", &s[target].eng);
            printf("%d번 학생의 수학 성적을 입력해주세요: ", target+1);
            scanf("%d", &s[target].math);
            printf("%d번 학생의 과학 성적을 입력해주세요: ", target+1);
            scanf("%d", &s[target].sci);  
        } else {
            printf("프로그램을 종료합니다.");
            break;
        }
    }

    return 0;
}
