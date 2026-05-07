#ifndef HELPERS_H
#define HELPERS_H

#include "operations.h"

typedef enum { LESS_THAN = -1, EQUAL, GREATER_THAN } Comparison;

Number *add_magnitudes(Number *, Number *);
Number *sub_magnitudes(Number *, Number *);
Number *mul_magnitudes(Number *, Number *);
Status divide_magnitudes(Number *num1, Number *num2, Number *result);
int compare_magnitudes(Number *, Number *);
void trim_zeroes(Number *);

#endif // !HELPERS_H
