#ifndef STACK_H
#define STACK_H

#define STACK_UNDERFLOW   3
#define STACK_OVERFLOW    4

typedef struct stack {
  int value;
  struct stack *tail;
} stack_oleks;

int stack_push(stack_oleks **stack, int value);

int stack_peek(stack_oleks **stack, int *value);

int stack_pop(stack_oleks **stack, int *value);

#endif // STACK_H
