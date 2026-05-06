#ifndef HELPERS_H
#define HELPERS_H

#include "operations.h"

Number *add_magnitudes(Number *, Number *);
Number *sub_magnitudes(Number *, Number *);
Number *mul_magnitudes(Number *, Number *);
int compare_magnitudes(Number *, Number *);
void trim_zeroes(Number *);

#endif // !HELPERS_H
