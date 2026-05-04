#include "operations.h"
#include <stdio.h>

int main() {
    char operations;
    do {
        scanf("%c", &operations);
        switch (operations) {
        case '+':
            addition();
            break;
        case '-':
            subtraction();
            break;
        case '*':
            multiplication();
            break;
        case '/':
            division();
            break;
        default:
            break;
        }
    } while (operations != 'Q');
}
