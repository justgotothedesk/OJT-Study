#include <stdio.h>
#include <pthread.h>

#define SIZE 5
#define NUM_THREADS 5

extern pthread_mutex_t print_mutex;

typedef struct {
	int data[SIZE];
	int rear, front;
} Queue;

void init(Queue *q);
int is_empty(Queue *q);
int is_full(Queue *q);
void enqueue(Queue *q, int data);
int dequeue(Queue *q);
void *worker(void *param);
