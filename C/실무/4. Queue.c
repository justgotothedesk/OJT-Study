#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 5
#define NUM_THREADS 5

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int data[SIZE];
    int rear, front;
} Queue;

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
            // printf("Thread %ld dequeue: %d at index %d\n", pthread_self(), data, q->front); \
            pthread_self() 메서드 호출 시, queue의 값이 아닌 엉뚱한 값인 0이 출력이 됨 \
            이는 pthread_self()가 반환하는 값이 매우 크고, 이 값이 스택에 저장될 때, \
            data 값이 위치한 메모리와 충돌하여 data 값이 덮어쓰여지는 것으로 추정됨. \
            따라서 출력 시에, data를 먼저 출력하고 thread_id를 출력하여 값이 덮어지는 것을 방지.
        } else {
            break;
        }
        sleep(1);
    }

    return NULL;
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

    pthread_mutex_destroy(&print_mutex);

    return 0;
}
