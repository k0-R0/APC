#include "helpers.h"
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
    LL *new = calloc(1, sizeof(LL));
    new->data = digit;
    new->next = num->head;
    if (num->head)
        num->head->prev = new;
    if (!num->tail)
        num->tail = new;
    num->head = new;
}

void append_digit(Number *num, int digit) {
    LL *new = calloc(1, sizeof(LL));
    new->data = digit;
    new->prev = num->tail;
    if (num->tail)
        num->tail->next = new;
    if (!num->head)
        num->head = new;
    num->tail = new;
}

int get_length(Number *num1) {
    LL *head = num1->head;
    int length = 0;
    while (head) {
        head = head->next;
        length++;
    }
    return length;
}

int compare_magnitudes(Number *num1, Number *num2) {
    int len1 = get_length(num1);
    int len2 = get_length(num2);
    if (len1 > len2)
        return 1;
    else if (len1 < len2)
        return -1;
    else {
        LL *head1 = num1->head;
        LL *head2 = num2->head;
        while (head1) {
            if (head1->data > head2->data)
                return 1;
            else if (head1->data < head2->data)
                return -1;
            head1 = head1->next;
            head2 = head2->next;
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

    while (tail1) {
        int diff = tail1->data - borrow;

        if (tail2) {
            diff -= tail2->data;
            tail2 = tail2->prev;
        }

        if (diff < 0) {
            borrow = 1;
            diff += 10;
        } else {
            borrow = 0;
        }

        prepend_digit(result, diff);

        tail1 = tail1->prev;
    }
    if (result->head == NULL)
        prepend_digit(result, 0);
    return result;
}

Status mul_digit(Number *num, Number *result, int digit) {
    LL *tail = num->tail;
    int carry = 0;
    while (tail) {
        int product = carry + digit * tail->data;
        carry = product / 10;
        product %= 10;
        prepend_digit(result, product);
        tail = tail->prev;
    }
    if (carry)
        prepend_digit(result, carry);
    return SUCCESS;
}

Number *mul_magnitudes(Number *num1, Number *num2) {
    Number *result = calloc(1, sizeof(Number));
    LL *tail2 = num2->tail;

    int digits_pos = 0;
    while (tail2) {
        int digit = tail2->data;
        // keep track of digits position
        Number *position_adjusted_num = calloc(1, sizeof(Number));
        for (int i = 0; i < digits_pos; i++)
            prepend_digit(position_adjusted_num, 0);
        digits_pos++;
        // multiply function for 1 digit
        if (mul_digit(num1, position_adjusted_num, digit) == FAILURE)
            exit(0);
        // add multiplication result to current result
        Number *digit_product = addition(result, position_adjusted_num);
        // to check if something broke
        free_num(result);
        free_num(position_adjusted_num);
        result = digit_product;
        tail2 = tail2->prev;
    }
    return result;
}

int find_cofactor(Number **dividend_ptr, Number *divisor) {
    for (int i = 9; i >= 0; i--) {
        Number *multiple = calloc(1, sizeof(Number));
        multiple->sign = 1;
        mul_digit(divisor, multiple, i);
        trim_zeroes(multiple);

        if (compare_magnitudes(*dividend_ptr, multiple) >= 0) {
            Number *remainder = sub_magnitudes(*dividend_ptr, multiple);
            remainder->sign = 1;
            free_num(multiple);
            free_num(*dividend_ptr);
            *dividend_ptr = remainder;
            return i;
        }
        free_num(multiple);
    }
    return 0;
}

Status divide_magnitudes(Number *num1, Number *num2, Number *result) {
    int cmp = compare_magnitudes(num1, num2);
    if (cmp == EQUAL) {
        append_digit(result, 1);
        return SUCCESS;
    } else if (cmp == LESS_THAN) {
        append_digit(result, 0);
        return SUCCESS;
    }

    Number *dividend = calloc(1, sizeof(Number));
    dividend->sign = 1;
    LL *head = num1->head;

    while (head) {
        append_digit(dividend, head->data);
        trim_zeroes(dividend);

        if (compare_magnitudes(dividend, num2) >= 0) {
            int cofactor = find_cofactor(&dividend, num2);
            append_digit(result, cofactor);
        } else {
            append_digit(result, 0);
        }
        head = head->next;
    }

    free_num(dividend);
    trim_zeroes(result);
    return SUCCESS;
}
