#include <stdlib.h>
#include "queue.h"


int queue_init(struct queue *queue) {
  queue = queue;
  return 0;
}

int queue_push(struct queue* queue, int pri) {
  struct queue* new_queue;
  new_queue = malloc(sizeof(struct queue));

  if (new_queue == NULL) {
  	return STACK_OVERFLOW;
  }
  new_queue -> value = pri;
  new_queue -> tail = queue;  //ny pointer
  if( new_queue -> value > queue -> value){
    // new_queue øverst, 
  }
  else {
    // new_queue skal sammenlignes med næste værdi, hvis queue peger på null så done
  }
  
  
  return 0;
}

int queue_pop(struct queue* queue, int *pri_ptr) {
  struct queue *old_queue;

  if (queue == NULL) {
  	return STACK_UNDERFLOW;
  }

  old_queue = queue;

  *pri_ptr = old_queue-> value;
  queue   = old_queue-> tail;

  free(old_queue);

  return 0;
}

int queue_destroy(struct queue** queue) {
	struct queue *old_queue;

	old_queue = *queue;

	if (old_queue == NULL) {
		free(old_queue);
		return 0;
	} else {
		*queue = old_queue -> tail;
		free(old_queue);
		queue_destroy(queue);
	}

  return 0;
}
