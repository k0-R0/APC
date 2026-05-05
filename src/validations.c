#include "operations.h"
#include <string.h>

Status validate_num(char *num) {
    if (!num)
        return FAILURE;
    if (num[0] == '+' || num[0] == '-')
        num++;
    if (*num == '\0')
        return FAILURE;
    size_t digit_length = strspn(num, "0123456789");
    if (num[digit_length] == '\0')
        return SUCCESS;
    return FAILURE;
}
