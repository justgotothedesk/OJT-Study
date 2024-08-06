#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define INCREMENT_COUNT 100000000
#define THREAD_COUNT 8
#define EXPECTED_RESULT 800000000

// Regular Global Variable
int global_var = 0;

void *increment_global_var(void *args) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        global_var++;
    }
}

// Regular Local Variable
void *increment_local_var(void *args) {
    int local_var = 0;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        local_var++;
    }
    *(int *)args = local_var;
}

// Atomic Global Variable
atomic_int atomic_global_var = 0;

void *increment_atomic_global_var(void *args) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        atomic_fetch_add(&atomic_global_var, 1);
    }
}

// Atomic Local Variable
void *increment_atomic_local_var(void *args) {
    atomic_int atomic_local_var = 0;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        atomic_fetch_add(&atomic_local_var, 1);
    }
    *(atomic_int *)args = atomic_local_var;
}

// Spinlock Variable
typedef struct {
    int value;
    pthread_spinlock_t spinlock;
} SpinlockVariable;

void *increment_spinlock_var(void *args) {
    SpinlockVariable *var = (SpinlockVariable *)args;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        pthread_spin_lock(&var->spinlock);
        var->value++;
        pthread_spin_unlock(&var->spinlock);
    }
}

// Semaphore Variable
typedef struct {
    int value;
    sem_t semaphore;
} SemaphoreVariable;

void *increment_semaphore_var(void *args) {
    SemaphoreVariable *var = (SemaphoreVariable *)args;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        sem_wait(&var->semaphore);
        var->value++;
        sem_post(&var->semaphore);
    }
}

// Array Variable
int array[THREAD_COUNT*8] = {0};

void *increment_array_var(void *args) {
    int index = *(int *)args;
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        array[index*8]++;
    }
}

// TLS Variable
__thread int tls_variable = 0;

void *increment_tls_var(void *args) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        tls_variable++;
    }
    *(int *)args = tls_variable;
}

void run() {
    pthread_t threads[THREAD_COUNT];
    clock_t start, end;
    double cpu_time;
    
    printf("==========   Thread num: %d   ==========\n", THREAD_COUNT);

    // Regular Global Variable Test
    global_var = 0;
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_global_var, NULL);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Regular Global Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expected Value: %d\n\n", global_var, EXPECTED_RESULT);

    // Regular Local Variable Test
    int local_vars[THREAD_COUNT] = {0};
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_local_var, &local_vars[i]);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    int total_local_var = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        total_local_var += local_vars[i];
    }
    printf("Regular Local Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expected Value: %d\n\n", total_local_var, EXPECTED_RESULT);

    // TLS Variable Test
    int tls_collector[THREAD_COUNT] = {0};
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_tls_var, &tls_collector[i]);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
    int total_tls_var = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        total_tls_var += tls_collector[i];
    }
    printf("TLS Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expected Value: %d\n\n", total_tls_var, EXPECTED_RESULT);

    // Atomic Global Variable Test
    atomic_store(&atomic_global_var, 0);
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_atomic_global_var, NULL);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Atomic Global Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expected Value: %d\n\n", atomic_global_var, EXPECTED_RESULT);

    // Atomic Local Variable Test
    atomic_int atomic_local_vars[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) {
        atomic_store(&atomic_local_vars[i], 0);
    }
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_atomic_local_var, &atomic_local_vars[i]);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    int total_atomic_local_var = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        total_atomic_local_var += atomic_load(&atomic_local_vars[i]);
    }
    printf("Atomic Local Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expected Value: %d\n\n", total_atomic_local_var, EXPECTED_RESULT);

    // Spinlock Variable Test
    SpinlockVariable spinlock_var = {0, 0};
    pthread_spin_init(&spinlock_var.spinlock, PTHREAD_PROCESS_PRIVATE);
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_spinlock_var, &spinlock_var);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Spinlock Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expected Value: %d\n\n", spinlock_var.value, EXPECTED_RESULT);
    pthread_spin_destroy(&spinlock_var.spinlock);

    // Semaphore Variable Test
    SemaphoreVariable semaphore_var = {0};
    sem_init(&semaphore_var.semaphore, 0, 1);
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, increment_semaphore_var, &semaphore_var);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Semaphore Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expected Value: %d\n\n", semaphore_var.value, EXPECTED_RESULT);
    sem_destroy(&semaphore_var.semaphore);

    // Array Variable Test
    int indices[THREAD_COUNT];
    start = clock();
    for (int i = 0; i < THREAD_COUNT; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, increment_array_var, &indices[i]);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
    int total_array_var = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        total_array_var += array[i*8];
    }
    printf("Array Variable\n");
    printf("Time: %f seconds\n", cpu_time);
    printf("Value: %d. Expectd Value: %d\n\n", total_array_var, EXPECTED_RESULT);

    printf("========================================\n\n");
}

int main() {
    run();

    return 0;
}