#include "operations.h"
#include <stdio.h>

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
    Number num1, num2;
    if (create_num(&num1, a) == FAILURE)
        return 0;
    if (create_num(&num2, b) == FAILURE)
        return 0;
    Number *result;
    switch (*operation) {
    case '+':
        result = addition(&num1, &num2);
        print_num(result);
        break;
    case '-':
        result = subtraction(&num1, &num2);
        print_num(result);
        break;
    case '*':
        result = multiplication(&num1, &num2);
        print_num(result);
        break;
    case '/':
        // division();
        break;
    default:
        break;
    }
}
