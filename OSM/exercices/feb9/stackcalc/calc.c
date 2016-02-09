#include <stdio.h>
#include <ctype.h>

#include "stack.h"

#define EXIT_OP     1
#define UNKNOWN_OP  2

int performOp(stack_oleks **stack, char op) {
  int retval;
  int value, x, y;

  switch (op) {
  case '*':
    retval = stack_pop(stack, &x);
    if (retval != 0) break;

    retval = stack_pop(stack, &y);
    if (retval != 0) break;

    retval = stack_push(stack, x * y);
    break;
  case 'e':
    retval = EXIT_OP;
    break;
  case 'p':
    retval = stack_peek(stack, &value);
    if (retval != 0) break;
    printf("%d\n", value);
    break;
  default:
    retval = UNKNOWN_OP;
  }

  return retval;
}

// returns an integer indicating whether we should continue to interact
int interact(stack_oleks **stack) {
  int value;
  char op;
  int retval;

  printf("> ");

  if (scanf("%d", &value) == 1) {
    retval = stack_push(stack, value);
  } else if (scanf("%c", &op) == 1) {
    if (!isspace(op)) {
      retval = performOp(stack, op);
    }
  }

  return retval;
}



int main() {
  stack_oleks *stack = NULL;

  int retval;
  char cont;

  cont = 1;
  while(cont) {
    retval = interact(&stack);
    switch (retval) {
    case EXIT_OP:
      cont = 0;
      break;
    case UNKNOWN_OP:
      printf("Unknown operator!\n");
      break;
    case STACK_UNDERFLOW:
      printf("Stack underflow!\n");
      break;
    case STACK_OVERFLOW:
      printf("Stack overflow!\n");
      break;
    default:
      continue;
    }
  }

  return 0;
}
