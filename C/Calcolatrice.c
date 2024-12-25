#include <stdio.h>

int main() {
    printf("Welcome to calculator\n");
    printf("this code was made by ClopeziohH\n");
    int fnum, snum;
    char operator;
    printf("Enter first number: ");
    scanf("%d", &fnum);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);
    printf("Enter second number: ");
    scanf("%d", &snum);

    if (operator == '+') {
        printf("the result is: %d", fnum + snum);
    } else if (operator == '-') {
        printf("the result is: %d", fnum - snum);
    } else if (operator == '*') {
        printf("the result is: %d", fnum * snum);
    } else if (operator == '/') {
        printf("the result is: %d", fnum / snum);
    } else {
        printf("Invalid operator\n");
    }
    }