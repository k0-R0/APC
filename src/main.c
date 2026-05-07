#include "operations.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Error %d", argc);
        return 0;
    }
    char *a = argv[1];
    char *b = argv[3];
    char *operation = argv[2];
    if (validate_num(a) == FAILURE)
        return 0;
    if (validate_num(b) == FAILURE)
        return 0;
    Number *num1 = malloc(sizeof(Number));
    Number *num2 = malloc(sizeof(Number));
    if (create_num(num1, a) == FAILURE)
        return 0;
    if (create_num(num2, b) == FAILURE)
        return 0;
    Number *result = calloc(1, sizeof(Number));
    switch (*operation) {
    case '+':
        result = addition(num1, num2);
        print_num(result);
        break;
    case '-':
        result = subtraction(num1, num2);
        print_num(result);
        break;
    case 'x': {
        if (multiplication(num1, num2, result) == FAILURE) {
            printf("Multiplication Failed.");
            break;
        };
        print_num(result);
        break;
    }
    case '/': {
        if (division(num1, num2, result) == DIVISON_BY_ZERO) {
            printf("Attempt to divide by Zero.\n");
            break;
        }
        print_num(result);
        break;
    }
    default:
        break;
    }
    free_num(num1);
    free_num(num2);
    free_num(result);
}
