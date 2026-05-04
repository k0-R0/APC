#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a > b) ? (a) : (b))

void addition() {
    char *a = malloc(200);
    char *b = malloc(200);
    char *result = malloc(201);
    scanf("%s %s", a, b);
    int len_a = strlen(a);
    int len_b = strlen(b);
    int carry = 0;
    int a_iter = len_a - 1;
    int b_iter = len_b - 1;
    int result_iter = max(len_a, len_b);
    while (a_iter >= 0 || b_iter >= 0) {
        int sum = carry;
        if (a[a_iter])
            sum += a[a_iter] - '0';
        if (b[b_iter])
            sum += b[b_iter] - '0';
        result[result_iter--] = sum % 10 + '0';
        carry = sum / 10;
        a_iter--;
        b_iter--;
    }
    if (carry)
        result[result_iter] = carry;
    (carry) ? printf("%s\n", result) : printf("%s\n", result + 1);
}
