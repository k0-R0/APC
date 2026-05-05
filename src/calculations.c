#include "operations.h"
#include <stdio.h>
#include <stdlib.h>

Status create_num(Number *num, char *str) {
    LL *prev = NULL;
    num->sign = 1;
    if (str[0] == '-' || str[0] == '+') {
        num->sign = (str[0] == '-') ? -1 : 1;
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
    return SUCCESS;
}

void print_num(Number *num) {
    LL *head = num->head;
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
}

Number *addition(Number *num1, Number *num2) {
    Number *result = calloc(1, sizeof(Number));

    LL *tail1 = num1->tail;
    LL *tail2 = num2->tail;

    int carry = 0;
    LL *next = NULL;

    while (tail1 || tail2) {
        LL *new = calloc(1, sizeof(LL));
        int sum = 0;
        if (tail1)
            sum += tail1->data;

        if (tail2)
            sum += tail2->data;

        new->data = sum % 10;
        carry = sum / 10;

        if (!tail1->next && !tail2->next)
            result->tail = new;

        new->next = next;
        if (next)
            next->prev = new;
        next = new;

        if (tail1)
            tail1 = tail1->prev;
        if (tail2)
            tail2 = tail2->prev;
    }
    if (carry) {
        LL *new = calloc(1, sizeof(LL));
        new->data = carry;
        new->next = next;
        if (next)
            next->prev = new;
        next = new;
    }
    result->head = next;
    return result;
}

Number *subtraction(Number *num1, Number *num2) {
    Number *result = calloc(1, sizeof(Number));
    return result;
}
