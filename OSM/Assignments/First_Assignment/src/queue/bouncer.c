#include <stdio.h>  // printf, scanf, stdin
#include <ctype.h>  // isspace
#include "queue.h"


static inline char
skip_spaces() {
  char c;

  do {
    c = getc(stdin);
  } while (isspace(c));

  return c;
}

static inline void
skip_until_space() {
  char c;

  do {
    c = getc(stdin);
  } while (!(isspace(c) || c == EOF));
}

void
loop(struct queue *queue) {
  char op = 0;
  int pri = 0;

  while(1) {

    op = skip_spaces();

    switch (op) {
    case EOF:
      return;
      break;

    case 'p':
      if (0) { queue_pop(&queue, &pri);
        printf("!! Queue underflow.\n");
      } else {
        printf("=> %d\n", pri);
      }
      break;

    default:
      ungetc(op, stdin);
      if (scanf("%d", &pri) == 1) {
        if (0) { queue_push(&queue, pri);
          printf("!! Queue overflow.\n");
        }
      } else {
        skip_until_space();
        printf("Invalid input!\n");
      }
    }
  }
}

void
shutdown() {
  // TODO: Pop everything off of the queue.
}

int
main() {
  struct queue *queue = NULL;

  loop(queue);

  shutdown();



  return 0;
}
