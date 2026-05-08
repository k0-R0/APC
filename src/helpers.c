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

Status prepend_digit(Number *num, int digit) {
    LL *new = calloc(1, sizeof(LL));
    if (!new)
        return FAILURE;
    new->data = digit;
    new->next = num->head;
    if (num->head)
        num->head->prev = new;
    if (!num->tail)
        num->tail = new;
    num->head = new;
    return SUCCESS;
}

Status append_digit(Number *num, int digit) {
    LL *new = calloc(1, sizeof(LL));
    if (!new)
        return FAILURE;
    new->data = digit;
    new->prev = num->tail;
    if (num->tail)
        num->tail->next = new;
    if (!num->head)
        num->head = new;
    num->tail = new;
    return SUCCESS;
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

Status add_magnitudes(Number *num1, Number *num2, Number *result) {
    // Number *result = calloc(1, sizeof(Number));
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

        if (prepend_digit(result, digit) == FAILURE)
            return FAILURE;

        if (tail1)
            tail1 = tail1->prev;
        if (tail2)
            tail2 = tail2->prev;
    }
    if (carry) {
        if (prepend_digit(result, carry) == FAILURE)
            return FAILURE;
    }
    return SUCCESS;
}

Status sub_magnitudes(Number *num1, Number *num2, Number *result) {
    // Number *result = calloc(1, sizeof(Number));
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

        if (prepend_digit(result, diff) == FAILURE)
            return FAILURE;

        tail1 = tail1->prev;
    }
    if (result->head == NULL)
        if (prepend_digit(result, 0) == FAILURE)
            return FAILURE;
    return SUCCESS;
}

Status mul_digit(Number *num, Number *result, int digit) {
    LL *tail = num->tail;
    int carry = 0;
    while (tail) {
        int product = carry + digit * tail->data;
        carry = product / 10;
        product %= 10;
        if (prepend_digit(result, product) == FAILURE)
            return FAILURE;
        tail = tail->prev;
    }
    if (carry)
        if (prepend_digit(result, carry) == FAILURE)
            return FAILURE;
    return SUCCESS;
}

Status mul_magnitudes(Number *num1, Number *num2, Number *result) {
    LL *tail2 = num2->tail;

    int digits_pos = 0;
    while (tail2) {
        int digit = tail2->data;
        // keep track of digits position
        Number *position_adjusted_num = calloc(1, sizeof(Number));
        for (int i = 0; i < digits_pos; i++)
            if (prepend_digit(position_adjusted_num, 0) == FAILURE) {
                free_num(position_adjusted_num);
                return FAILURE;
            }
        digits_pos++;
        // multiply function for 1 digit
        if (mul_digit(num1, position_adjusted_num, digit) == FAILURE) {
            free_num(position_adjusted_num);
            return FAILURE;
        }
        // add multiplication result to current result
        Number *temp_sum = calloc(1, sizeof(Number));
        if (addition(result, position_adjusted_num, temp_sum) == FAILURE) {
            free_num(position_adjusted_num);
            free_num(temp_sum);
            return FAILURE;
        }
        // to check if something broke
        // clear result nodes and swap it with temp_sum
        free_number_nodes(result);
        result->head = temp_sum->head;
        result->tail = temp_sum->tail;

        temp_sum->head = temp_sum->tail = NULL;
        free_num(temp_sum);
        free_num(position_adjusted_num);
        tail2 = tail2->prev;
    }
    return SUCCESS;
}

Status find_cofactor(Number **dividend_ptr, Number *divisor, int *cofactor) {
    for (int i = 9; i >= 0; i--) {
        Number *multiple = calloc(1, sizeof(Number));
        multiple->sign = 1;
        if (mul_digit(divisor, multiple, i) == FAILURE) {
            free_num(multiple);
            return FAILURE;
        }
        trim_zeroes(multiple);

        if (compare_magnitudes(*dividend_ptr, multiple) >= 0) {
            Number *remainder = calloc(1, sizeof(Number));
            if (sub_magnitudes(*dividend_ptr, multiple, remainder) == FAILURE) {
                free_num(multiple);
                free_num(remainder);
                return FAILURE;
            }
            remainder->sign = 1;
            free_num(multiple);
            free_num(*dividend_ptr);
            *dividend_ptr = remainder;
            *cofactor = i;
            return SUCCESS;
        }
        free_num(multiple);
    }
    return FAILURE;
}

Status divide_magnitudes(Number *num1, Number *num2, Number *result) {
    int cmp = compare_magnitudes(num1, num2);
    if (cmp == EQUAL) {
        if (append_digit(result, 1) == FAILURE)
            return FAILURE;
        return SUCCESS;
    } else if (cmp == LESS_THAN) {
        if (append_digit(result, 0) == FAILURE)
            return FAILURE;
        return SUCCESS;
    }

    Number *dividend = calloc(1, sizeof(Number));
    dividend->sign = 1;
    LL *head = num1->head;

    while (head) {
        if (append_digit(dividend, head->data) == FAILURE) {
            free_num(dividend);
            return FAILURE;
        }
        trim_zeroes(dividend);

        if (compare_magnitudes(dividend, num2) >= 0) {
            int cofactor;
            if (find_cofactor(&dividend, num2, &cofactor) == FAILURE) {
                free_num(dividend);
                return FAILURE;
            }
            if (append_digit(result, cofactor) == FAILURE) {
                free_num(dividend);
                return FAILURE;
            }
        } else {
            if (append_digit(result, 0) == FAILURE) {
                free_num(dividend);
                return FAILURE;
            }
        }
        head = head->next;
    }

    free_num(dividend);
    trim_zeroes(result);
    return SUCCESS;
}
