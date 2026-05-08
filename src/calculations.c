#include "helpers.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>

Status create_num(Number *num, char *str) {
    LL *prev = NULL;
    num->sign = 1;
    if (str[0] == '-' || str[0] == '+') {
        if (str[0] == '-')
            num->sign = -1;
        str++;
    }
    for (int i = 0; str[i]; i++) {
        LL *new = calloc(1, sizeof(LL));
        if (!new)
            return FAILURE;
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
    printf("\n");
}

void free_number_nodes(Number *num) {
    if (!num)
        return;
    while (num->head) {
        LL *to_free = num->head;
        num->head = to_free->next;
        free(to_free);
    }
    num->head = num->tail = NULL;
}

void free_num(Number *num) {
    free_number_nodes(num);
    free(num);
}

Status addition(Number *num1, Number *num2, Number *result) {
    if (num1->sign == num2->sign) {
        if (add_magnitudes(num1, num2, result) == FAILURE)
            return FAILURE;
        result->sign = num1->sign;
    } else {
        int cmp = compare_magnitudes(num1, num2);
        if (cmp == GREATER_THAN || cmp == EQUAL) {
            if (sub_magnitudes(num1, num2, result) == FAILURE)
                return FAILURE;
            result->sign = num1->sign;
        } else {
            if (sub_magnitudes(num2, num1, result) == FAILURE)
                return FAILURE;
            result->sign = num2->sign;
        }
    }

    trim_zeroes(result);
    return SUCCESS;
}

Status subtraction(Number *num1, Number *num2, Number *result) {
    num2->sign *= -1;
    if (addition(num1, num2, result) == FAILURE)
        return FAILURE;
    return SUCCESS;
}

Status multiplication(Number *num1, Number *num2, Number *result) {
    if (mul_magnitudes(num1, num2, result) == FAILURE)
        return FAILURE;
    result->sign = (num1->sign == num2->sign) ? 1 : -1;
    return SUCCESS;
}

Status division(Number *num1, Number *num2, Number *result) {
    if (num2->head->data == 0)
        return DIVISON_BY_ZERO;
    if (divide_magnitudes(num1, num2, result) == FAILURE)
        return FAILURE;
    result->sign = (num1->sign == num2->sign) ? 1 : -1;
    return SUCCESS;
}
