#ifndef OPERATIONS_H
#define OPERATIONS_H

typedef struct LinkedList {
    int data;
    struct LinkedList *next;
    struct LinkedList *prev;
} LL;

typedef struct Number {
    LL *head;
    LL *tail;
    int sign;
    char *str;
} Number;

typedef enum { SUCCESS, FAILURE } Status;

Status create_num(Number *, char *);
void print_num(Number *);
Status validate_num(char *num);
Number *addition(Number *, Number *);
Number *subtraction(Number *, Number *);
Number *multiplication(Number *, Number *);
void division();
#endif // !OPERATIONS_H
