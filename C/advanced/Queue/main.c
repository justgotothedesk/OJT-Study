#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "define.h"

pthread_mutex_t print_mutx = PTHREAD_MUTEX_INITIALIZER;

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
