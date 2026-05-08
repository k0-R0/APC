#include "helpers.h"
#include "operations.h"
#include <stdlib.h>
#include <string.h>

void trim_zeroes(Number *num) {
    // trim zeroes till we are at a non zero digit
    // or we are the last digit
    if (!num || !num->head)
        return;
    while (num->head->next && num->head->data == 0) {
        LL *to_free = num->head;
        num->head = num->head->next;
        num->head->prev = NULL;
        free(to_free);
    }
    // if last digit is 0 set sign to +ve
    // helps handling add/sub properly
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
    // compare lengths first
    int len1 = get_length(num1);
    int len2 = get_length(num2);
    if (len1 > len2)
        return 1;
    else if (len1 < len2)
        return -1;
    else {
        // if lengths are same then compare digits from the
        // highest place
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
    // start from the last digit (lowest place)
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

        // move to the next digit
        if (tail1)
            tail1 = tail1->prev;
        if (tail2)
            tail2 = tail2->prev;
    }
    if (carry) {
        // if there is an extra digit at the end
        if (prepend_digit(result, carry) == FAILURE)
            return FAILURE;
    }
    return SUCCESS;
}

Status sub_magnitudes(Number *num1, Number *num2, Number *result) {
    // start from the last digit (lowest place)
    LL *tail1 = num1->tail;
    LL *tail2 = num2->tail;

    int borrow = 0;

    // we iterate only through tail1 because
    // we call sub_magnitudes on num1 >= num2
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
    // multiply 1 digit to the number
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
    trim_zeroes(result);
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

        // clear result nodes and swap it with temp_sum
        free_number_nodes(result);
        result->head = temp_sum->head;
        result->tail = temp_sum->tail;

        // set temp_sum head,tail to null so that
        // free_number_nodes doesn't free our result
        temp_sum->head = temp_sum->tail = NULL;

        free_num(temp_sum);
        free_num(position_adjusted_num);

        tail2 = tail2->prev;
    }
    return SUCCESS;
}

Status find_cofactor(Number **dividend_ptr, Number *divisor, int *cofactor) {
    // find cofactor for a dividend, divisor
    // iterate from 9 to 0, the moment we find a multiple <= dividend
    // we have our cofactor
    for (int i = 9; i >= 0; i--) {
        // get multiple by multiplying cofactor to divisor
        Number *multiple = calloc(1, sizeof(Number));
        multiple->sign = 1;
        if (mul_digit(divisor, multiple, i) == FAILURE) {
            free_num(multiple);
            return FAILURE;
        }

        // if our mutliple is smaller save cofactor, update dividend to
        // remainder and exit
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
    // we only proceed with doing the full long division
    // if num1 > num2, we optimize 0 and 1 case
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

    // we keep taking 1 digit from num1 and appending
    // to dividend, when dividend >= num2(divisor) we find the
    // cofactor
    Number *dividend = calloc(1, sizeof(Number));
    dividend->sign = 1;
    LL *head = num1->head;

    while (head) {
        if (append_digit(dividend, head->data) == FAILURE) {
            free_num(dividend);
            return FAILURE;
        }
        // IMPORTANT:
        // after appending digit trim zeroes because post first
        // division there's a chance that your num becomes 03
        // our comparison function uses length as a marker for comparison.
        trim_zeroes(dividend);

        if (compare_magnitudes(dividend, num2) >= 0) {
            int cofactor;
            if (find_cofactor(&dividend, num2, &cofactor) == FAILURE) {
                free_num(dividend);
                return FAILURE;
            }
            // append cofactor to result
            if (append_digit(result, cofactor) == FAILURE) {
                free_num(dividend);
                return FAILURE;
            }
        } else {
            // append 0 when the dividend is smaller
            // they'll get trimmed off if unnecessary but if not done
            // case like 100 / 2 will return 5
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
