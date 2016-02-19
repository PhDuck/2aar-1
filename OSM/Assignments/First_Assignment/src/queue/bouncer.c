#include <stdio.h>  // printf, scanf, stdin
#include <ctype.h>  // isspace
#include "queue.h"



void MergeSortSplit(struct queue* list,
          struct queue** frontQueue, struct queue** backQueue);
 
void MergeSort(struct queue** topPointer)
{
  struct queue* topEntry = *topPointer;
  struct queue* a;
  struct queue* b;
 
  if ((topEntry == NULL) || (topEntry->tail == NULL))
  {
    return;
  }
 
  MergeSortSplit(topEntry, &a, &b); 
  MergeSort(&a);
  MergeSort(&b);
  *topPointer = SortedMerge(a, b);
}
 
struct queue* SortedMerge(struct queue* a, struct queue* b)
{
  struct queue* result = NULL;

  if (a == NULL)
     return(b);
  else if (b == NULL)
     return(a);
 
  if (a->value > b->value)
  {
     result = a;
     result -> tail = SortedMerge(a -> tail, b);
  }
  else
  {
     result = b;
     result -> tail = SortedMerge(a, b -> tail);
  }
  return(result);
}
void MergeSortSplit(struct queue* list,
          struct queue** frontQueue, struct queue** backQueue)
{
  struct queue* listIterate;
  struct queue* midPointer;
  if (list==NULL || list->tail==NULL)
  {
    *frontQueue = list;
    *backQueue = NULL;
  }
  else
  {
    midPointer = list;
    listIterate = list->tail;
 
    while (listIterate != NULL)
    {
      listIterate = listIterate->tail;
      if (listIterate != NULL)
      {
        midPointer = midPointer->tail;
        listIterate = listIterate->tail;
      }
    } 
    *frontQueue = list;
    *backQueue = midPointer->tail;
    midPointer->tail = NULL;
  }
}

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
      if (0) { 
        printf("!! Queue underflow.\n");
      } else {
        queue_pop(&queue, &pri);
        printf("=> %d\n", pri);
      }
      break;

    default:
      ungetc(op, stdin);
      if (scanf("%d", &pri) == 1) {
        queue_push(&queue, pri);
        MergeSort(&queue);
        if (0) { 
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
