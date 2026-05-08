#ifndef HELPERS_H
#define HELPERS_H

#include "operations.h"

typedef enum { LESS_THAN = -1, EQUAL, GREATER_THAN } Comparison;

Status add_magnitudes(Number *num1, Number *num2, Number *result);
Status sub_magnitudes(Number *num1, Number *num2, Number *result);
Status mul_magnitudes(Number *num1, Number *num2, Number *result);
Status divide_magnitudes(Number *num1, Number *num2, Number *result);
Status append_digit(Number *num, int digit);
int compare_magnitudes(Number *, Number *);
void trim_zeroes(Number *);

#endif // !HELPERS_H
