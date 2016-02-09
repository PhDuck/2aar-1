#include <stdlib.h>
#include "stack.h"

int stack_push(stack_oleks **stack, int value) {
  stack_oleks *new_stack = (stack_oleks*)malloc(sizeof(stack_oleks));

  if (new_stack == NULL) {
    return STACK_OVERFLOW;
  }

  new_stack->value = value;
  new_stack->tail = *stack;

  *stack = new_stack;

  return 0;
}

int stack_peek(stack_oleks **stack, int *value) {
  if (*stack == NULL) {
    return STACK_UNDERFLOW;
  }

  *value = (*stack)->value;

  return 0;
}

int stack_pop(stack_oleks **stack, int *value) {
  stack_oleks *old_stack;

  if (*stack == NULL) {
    return STACK_UNDERFLOW;
  }

  old_stack = *stack;
  *value = old_stack->value;
  *stack = old_stack->tail;

  free(old_stack);

  return 0;
}
