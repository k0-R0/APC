#include "operations.h"
#include <stdlib.h>
#include <string.h>

void trim_zeroes(Number *num) {
    if (!num || !num->head)
        return;
    while (num->head->next && num->head->data == 0) {
        LL *to_free = num->head;
        num->head = num->head->next;
        num->head->prev = NULL;
        free(to_free);
    }
    if (num->head->data == 0)
        num->sign = 1;
}

void prepend_digit(Number *num, int digit) {
    LL *new = calloc(1, sizeof(Number));
    new->data = digit;
    new->next = num->head;
    if (num->head)
        num->head->prev = new;
    if (!num->tail)
        num->tail = new;
    num->head = new;
}

void trim_zeroes_string(char **str) {
    while (*(*str + 1) && **str == '0')
        (*str)++;
}

int compare_magnitudes(Number *num1, Number *num2) {
    trim_zeroes_string(&(num1->str));
    trim_zeroes_string(&(num2->str));
    char *str1 = num1->str;
    char *str2 = num2->str;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if (len1 > len2)
        return 1;
    else if (len1 < len2)
        return -1;
    else {
        for (int i = 0; str1[i]; i++) {
            if (str1[i] > str2[i])
                return 1;
            else if (str1[i] < str2[i])
                return -1;
            else
                continue;
        }
    }
    return 0;
}

Number *add_magnitudes(Number *num1, Number *num2) {
    Number *result = calloc(1, sizeof(Number));
    LL *tail1 = num1->tail;
    LL *tail2 = num2->tail;

    int carry = 0;

    while (tail1 || tail2) {
        int sum = carry;
        if (tail1)
            sum += tail1->data;

        if (tail2)
            sum += tail2->data;

        int digit = sum % 10;
        carry = sum / 10;

        prepend_digit(result, digit);

        if (tail1)
            tail1 = tail1->prev;
        if (tail2)
            tail2 = tail2->prev;
    }
    if (carry) {
        prepend_digit(result, carry);
    }
    return result;
}

Number *sub_magnitudes(Number *num1, Number *num2) {
    Number *result = calloc(1, sizeof(Number));
    LL *tail1 = num1->tail;
    LL *tail2 = num2->tail;

    int borrow = 0;

    while (tail1 || tail2) {
        int diff = -borrow;
        if (tail1)
            diff += tail1->data;

        if (tail2)
            diff -= tail2->data;

        if (diff < 0) {
            borrow = 1;
            diff += 10;
        }
        int digit = diff;
        if (!digit && !tail2)
            break;
        prepend_digit(result, digit);

        if (tail1)
            tail1 = tail1->prev;
        if (tail2)
            tail2 = tail2->prev;
    }
    return result;
}
