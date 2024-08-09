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
    return q->rear == SIZE-1;
}

void enqueue(Queue *q, int data) {
    if (is_full(q)) {
        printf("Queue is full\n");
    } else {
        q->rear++;
        q->data[q->rear] = data;
    }
}

int dequeue(Queue *q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
    } else {
        q->front++;

        return q->data[q->front];
    }
}

int main() {
    Queue circularQueue;

    return 0;
}