#include "operations.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char operations;
    do {
        scanf("%c", &operations);
        char *a = malloc(200);
        char *b = malloc(200);
        scanf("%s %s", a, b);

        if (validate_num(a) == FAILURE)
            return 0;
        if (validate_num(a) == FAILURE)
            return 0;
        Number num1, num2;
        if (create_num(&num1, a) == FAILURE)
            return 0;
        if (create_num(&num2, b) == FAILURE)
            return 0;
        Number *result;
        switch (operations) {
        case '+':
            result = addition(&num1, &num2);
            print_num(result);
            break;
        case '-':
            // subtraction();
            break;
        case '*':
            multiplication();
            break;
        case '/':
            division();
            break;
        default:
            break;
        }
    } while (operations != 'Q');
}
