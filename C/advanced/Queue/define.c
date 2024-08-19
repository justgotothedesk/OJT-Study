#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "define.h"
#include <stddef.h>

pthread_mutex_t print_mutex;

void init(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

int is_empty(Queue *q) {
    return q->front == q->rear;
}

int is_full(Queue *q) {
    return q->front == (q->rear+1)%SIZE;
}

void enqueue(Queue *q, int data) {
    pthread_mutex_lock(&print_mutex);
    if (!is_full(q)) {
        q->data[q->rear] = data;
        printf("Enqueue: %d at index %d\n", data, q->rear);
        q->rear = (q->rear + 1) % SIZE;
    } else {
        printf("Queue is full\n");
    }
    pthread_mutex_unlock(&print_mutex);
}

int dequeue(Queue *q) {
    pthread_mutex_lock(&print_mutex);
    int data;

    if (is_empty(q)) {
        printf("Thread %ld: Queue is empty\n", pthread_self());
        data = -1;
    }
    else {
        data = q->data[q->front];
        q->front = (q->front + 1) % SIZE;
    }
    pthread_mutex_unlock(&print_mutex);

    return data;
}

void *worker(void *param) {
    Queue *q = (Queue *)param;

    while (1) {
        int data = dequeue(q);
        if (data != -1) {
            printf("Dequeue: %d at index %d by Thread %ld\n", data, (q->front)-1, pthread_self());
        } else {
            break;
        }
        sleep(1);
    }

    return NULL;
}
