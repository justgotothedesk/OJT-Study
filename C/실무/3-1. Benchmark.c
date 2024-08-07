#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define INCREMENT_COUNT 1000000

// Normal 변수
typedef struct {
    int value;
} RegularVariable;

void *increment_regular_variable(void *args) {
    RegularVariable *var = (RegularVariable *)args;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        var->value++;
    }
}

// TLS 변수
__thread int tls_variable = 0;

void *increment_tls_variable(void *args) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        tls_variable++;
    }
}

// Spinlock 변수
typedef struct {
    int value;
    pthread_spinlock_t spinlock;
} SpinlockVariable;

void *increment_spinlock_variable(void *args) {
    SpinlockVariable *var = (SpinlockVariable *)args;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        pthread_spin_lock(&var->spinlock);
        var->value++;
        pthread_spin_unlock(&var->spinlock);
    }
}

// Semaphore 변수
typedef struct {
    int value;
    sem_t semaphore;
} SemaphoreVariable;

void *increment_semaphore_variable(void *args) {
    SemaphoreVariable *var = (SemaphoreVariable *) args;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        sem_wait(&var->semaphore);
        var->value++;
        sem_post(&var->semaphore);
    }
}

// Atomic 변수
typedef struct {
    atomic_int value;
} AtomicVariable;

void *increment_atomic_variable(void *args) {
    AtomicVariable *var = (AtomicVariable *) args;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        atomic_fetch_add(&var->value, 1);
    }
}

void run(int THREAD_COUNT) {
    pthread_t threads[THREAD_COUNT];
    clock_t start, end;
    double cpu_time;
    printf("==========   Thread num: %d   ==========\n", THREAD_COUNT);

    // Regular Variable Test
    RegularVariable regular_var = {0};
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_regular_variable, &regular_var);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Regular Variable Time: %f seconds\n", cpu_time);

    // TLS Variable Test
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_tls_variable, NULL);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("TLS Variable Time: %f seconds\n", cpu_time);

    // Spinlock Variable Test
    SpinlockVariable spinlock_var = {0, 0};
    pthread_spin_init(&spinlock_var.spinlock, PTHREAD_PROCESS_PRIVATE);
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_spinlock_variable, &spinlock_var);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Spinlock Variable Time: %f seconds\n", cpu_time);
    pthread_spin_destroy(&spinlock_var.spinlock);

    // Semaphore Variable Test
    SemaphoreVariable semaphore_var = {0};
    sem_init(&semaphore_var.semaphore, 0, 1);
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_semaphore_variable, &semaphore_var);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Semaphore Variable Time = %f seconds\n", cpu_time);
    sem_destroy(&semaphore_var.semaphore);

    // Atomic Variable Test
    AtomicVariable atomic_var = {0};
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_atomic_variable, &atomic_var);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Atomic Variable Time: %f seconds\n", cpu_time);
    printf("========================================\n\n");
}

int main() {
    run(2);
    run(4);
    run(8);
    
    return 0;
}