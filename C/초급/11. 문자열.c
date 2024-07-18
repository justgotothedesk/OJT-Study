#include <stdio.h>
#include <String.h>
#include <ctype.h>

int main() {
    char string[256];
    int count[256] = {0};
    int max = 0, min = 256;
    char max_char, min_char;    
    
    printf("문자열을 입력하세요: ");
    scanf("%s", string);

    int length = strlen(string);

    for (int i = 0; i < length; i++) {
        if(isalpha(string[i])) {
            count[tolower(string[i])]++;
        } else {
            count[string[i]]++;
        }
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] > 0) {
            if (max < count[i]) {
                max = count[i];
                max_char = i;
            }
            if (min > count[i]) {
                min = count[i];
                min_char = i;
            }
        }
    }

    printf("총 길이: %d, 최다 사용: %c (%d), 최소 사용: %c (%d)", length, max_char, max, min_char, min);

    return 0;
}