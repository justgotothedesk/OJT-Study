#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char file[100];
    char target[100];

    printf("파일 이름을 입력하세요: ");
    scanf("%s", file);
    printf("대문자로 변경하고자 하는 문자열을 입력하세요: ");
    scanf("%s", target);

    for (int i = 0; target[i] != '\0'; i++) {
        target[i] = toupper(target[i]);
    }

    FILE *f = fopen(file, "a");
    if (f == NULL) {
        perror("파일을 열 수 없습니다");
        return 1;
    }

    fputs(target, f);
    fclose(f);

    printf("대문자로 변경하여 [%s]에 저장하였습니다.\n", file);

    return 0;
}
