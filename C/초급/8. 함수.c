#include <stdio.h>
#include <stdlib.h>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        printf("오류: 0으로 나눌 수 없습니다.\n");
        exit(EXIT_FAILURE);
    }
    return a / b;
}

int main() {
    char expression[100];
    double num1, num2, result;
    char operator;

    printf("수식을 입력하세요: ");
    if (fgets(expression, sizeof(expression), stdin) == NULL) {
        printf("오류: 수식을 읽어올 수 없습니다.\n");
        return 1;
    }

    if (sscanf(expression, "%lf %c %lf", &num1, &operator, &num2) != 3) {
        printf("오류: 잘못된 수식입니다.\n");
        return 1;
    }

    switch (operator) {
        case '+':
            result = add(num1, num2);
            break;
        case '-':
            result = subtract(num1, num2);
            break;
        case '*':
            result = multiply(num1, num2);
            break;
        case '/':
            result = divide(num1, num2);
            break;
        default:
            printf("오류: 지원되지 않는 연산자입니다.\n");
            return 1;
    }

    printf("%s = %.2lf\n", expression, result);

    return 0;
}