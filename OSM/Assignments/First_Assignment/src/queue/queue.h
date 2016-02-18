#ifndef QUEUE_H
#define QUEUE_H

struct queue {
  int value;
  struct stack *tail;
};

#define STACK_UNDERFLOW   3
#define STACK_OVERFLOW    4

int queue_init(struct queue *queue);

int queue_push(struct queue *queue, int pri);

int queue_pop(struct queue *queue, int *pri_ptr);

int queue_destroy(struct queue *queue);

#endif /* QUEUE_H */
