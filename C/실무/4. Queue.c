#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 5
#define NUM_THREADS 5

typedef struct {
    int data[SIZE];
    int rear, front;
    pthread_mutex_t lock;
} Queue;

void init(Queue *q) {
    q->front = 0;
    q->rear = 0;
    pthread_mutex_init(&q->lock, NULL);
}

int is_empty(Queue *q) {
    return q->front == q->rear;
}

int is_full(Queue *q) {
    return q->front == (q->rear+1)%SIZE;
}

void enqueue(Queue *q, int data) {
    pthread_mutex_lock(&q->lock);

    if (!is_full(q)) {
        q->data[q->rear] = data;
        printf("Enqueue: %d at index %d\n", data, q->rear);
        q->rear = (q->rear+1)%SIZE;
    } else {
        printf("Queue is full\n");
    }

    pthread_mutex_unlock(&q->lock);
}

int dequeue(Queue *q) {
    pthread_mutex_lock(&q->lock);

    if (is_empty(q)) {
        printf("Thread %ld: Queue is empty\n", pthread_self());
        pthread_mutex_unlock(&q->lock);

        return -1;
    } else {
        int data = q->data[q->front];
        printf("Dequeue: %d from index %d\n", data, q->front);
        q->front = (q->front+1)%SIZE;
        pthread_mutex_unlock(&q->lock);

        return data;
    }
}

void *worker(void *param) {
    Queue *q = (Queue *) param;
    while (1) {
        int data = dequeue(q);
        if (data != -1) {
            printf("Thread %ld, data: %d\n", pthread_self(), data);
        } else {
            break;
        }
        sleep(1);
    }
}

int main() {
    Queue circularQueue;
    init(&circularQueue);

    int input;
    while (1) {
        printf("Enter data to enqueue (or -1 to quit): ");
        scanf("%d", &input);
        if ((input == -1) || (is_full(&circularQueue))) {
            break;
        }
        enqueue(&circularQueue, input);
    }

    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)&circularQueue);
    }

    sleep(2);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&circularQueue.lock);

    return 0;
}