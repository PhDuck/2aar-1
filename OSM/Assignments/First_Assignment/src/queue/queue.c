#include <stdlib.h>
#include "queue.h"
#include "bouncer.c"


int queue_init(struct queue *queue) {
  struct queue *queue = NULL;

  return 0;
}

int queue_push(struct queue *queue, int pri) {
  struct queue new_queue = (stuct queue*) malloc(sizeof(struct queue));

  new_queue -> value = pri;
  new_queue-> tail = *queue

  return 0;
}

int queue_pop(struct queue *queue, int *pri_ptr) {
  struct queue old_queue;

  old_queue = *queue;

  *pri_ptr = old_queue-> value;
  *queue = old_queue-> tail;

  free(old_queue);

  return 0;
}

int queue_destroy(struct queue *queue);
