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
} Number;

typedef enum { SUCCESS, FAILURE, DIVISON_BY_ZERO } Status;

Status create_num(Number *, char *);
void free_num(Number *);
void free_number_nodes(Number *num);
void print_num(Number *);
Status validate_num(char *num);
Number *addition(Number *, Number *);
Number *subtraction(Number *, Number *);
Status multiplication(Number *, Number *, Number *);
Status division(Number *, Number *, Number *);
#endif // !OPERATIONS_H
