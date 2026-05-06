#include "helpers.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>

Status create_num(Number *num, char *str) {
    LL *prev = NULL;
    num->sign = 1;
    num->str = str;
    if (str[0] == '-' || str[0] == '+') {
        if (str[0] == '-')
            num->sign = -1;
        str++;
    }
    for (int i = 0; str[i]; i++) {
        LL *new = calloc(1, sizeof(LL));
        new->data = str[i] - '0';
        new->prev = prev;
        if (i == 0)
            num->head = new;
        if (prev != NULL)
            prev->next = new;
        prev = new;
    }
    num->tail = prev;
    trim_zeroes(num);
    return SUCCESS;
}

void print_num(Number *num) {
    if (num->sign == -1)
        printf("-");
    LL *head = num->head;
    while (head) {
        printf("%d", head->data);
        head = head->next;
    }
}

Number *addition(Number *num1, Number *num2) {
    if (num1->head == NULL ||
        (num1->head->next == NULL && num1->head->data == 0))
        return num2;
    if (num2->head == NULL ||
        (num2->head->next == NULL && num2->head->data == 0))
        return num1;
    Number *result;
    if (num1->sign == num2->sign) {
        result = add_magnitudes(num1, num2);
        result->sign = num1->sign;
    } else {
        int cmp = compare_magnitudes(num1, num2);
        if (cmp >= 0) {
            result = sub_magnitudes(num1, num2);
            result->sign = num1->sign;
        } else {
            result = sub_magnitudes(num2, num1);
            result->sign = num2->sign;
        }
    }

    trim_zeroes(result);
    return result;
}

Number *subtraction(Number *num1, Number *num2) {
    num2->sign *= -1;
    return addition(num1, num2);
}

Number *multiplication(Number *num1, Number *num2) {
    Number *result = mul_magnitudes(num1, num2);
    result->sign = (num1->sign == num2->sign) ? 1 : -1;
    return result;
}
