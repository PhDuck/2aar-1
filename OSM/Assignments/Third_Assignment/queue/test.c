#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "queue.h"

#define NUM_THREADS 4

static struct queue queue;

void *pushing(void *arg) {

  printf("%s initialize \n", (char *) arg);
  queue_push(&queue, 2);
  queue_push(&queue, 2);
  queue_push(&queue, 2);
  printf("%s done \n", (char *) arg);
  return NULL;
}

void *pooping(void *arg) {
  int pri = 0;

  printf("%s initialize \n", (char *) arg);
  queue_pop(&queue, &pri);
  queue_pop(&queue, &pri);
  queue_pop(&queue, &pri);
  printf("%s done \n", (char *) arg);
  return NULL;
}

int main() {
  queue_init(&queue);
  pthread_t threads[NUM_THREADS];
  
  
  pthread_create(&threads[0], NULL, pushing, "thread 0");
  pthread_create(&threads[1], NULL, pooping, "thread 1"); 
  pthread_create(&threads[2], NULL, pushing, "thread 2");
  pthread_create(&threads[3], NULL, pooping, "thread 3"); 
  // join waits for the threads to finish
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  
  return 0;
}
