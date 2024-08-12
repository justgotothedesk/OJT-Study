#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 5

typedef struct {
    int data[SIZE];
    int rear, front;
} Queue;

void init(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int is_empty(Queue *q) {
    return q->front == q->rear;
}

int is_full(Queue *q) {
    return q->front == (q->rear+1)%SIZE;
}

void enqueue(Queue *q, int data) {
    if (is_full(q)) {
        printf("Queue is full\n");
    } else {
        q->rear = (q->rear+1)%SIZE;
        q->data[q->rear] = data;
    }
}

int dequeue(Queue *q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
    } else {
        q->front = (q->front+1)%SIZE;

        return q->data[q->front];
    }
}

int main() {
    Queue circularQueue;
    enqueue(circularQueue, 0);
    enqueue(circularQueue, 1);
    enqueue(circularQueue, 2);
    enqueue(circularQueue, 3);
    enqueue(circularQueue, 4);

    return 0;
}