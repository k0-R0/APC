#include "helpers.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>

Status create_num(Number *num, char *str) {
    num->head = num->tail = NULL;
    num->sign = 1;
    // default +ve , if symbol is present in string
    // move pointer to next index after storing sign
    if (str[0] == '-' || str[0] == '+') {
        if (str[0] == '-')
            num->sign = -1;
        str++;
    }
    // for every digit create a node and append
    // to the end
    for (int i = 0; str[i]; i++) {
        if (append_digit(num, str[i] - '0') == FAILURE)
            return FAILURE;
    }
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
    // traverse each node and free it
    while (num->head) {
        LL *to_free = num->head;
        num->head = to_free->next;
        free(to_free);
    }
    num->head = num->tail = NULL;
}

void free_num(Number *num) {
    // free the nodes first and then the num
    free_number_nodes(num);
    free(num);
}

Status addition(Number *num1, Number *num2, Number *result) {
    // if sign is same then result sign is same
    // else it is the sign of the bigger num
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
    // subtraction is just addition with a -ve sign
    num2->sign *= -1;
    if (addition(num1, num2, result) == FAILURE)
        return FAILURE;
    return SUCCESS;
}

Status multiplication(Number *num1, Number *num2, Number *result) {
    if (mul_magnitudes(num1, num2, result) == FAILURE)
        return FAILURE;
    // if sign is same sign will be +ve else -ve
    result->sign = (num1->sign == num2->sign) ? 1 : -1;
    return SUCCESS;
}

Status division(Number *num1, Number *num2, Number *result) {
    // handle division by 0
    if (num2->head->data == 0)
        return DIVISON_BY_ZERO;
    if (divide_magnitudes(num1, num2, result) == FAILURE)
        return FAILURE;
    // if sign is same sign will be +ve else -ve
    result->sign = (num1->sign == num2->sign) ? 1 : -1;
    return SUCCESS;
}
