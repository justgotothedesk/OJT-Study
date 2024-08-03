#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define INCREMENT_COUNT 1000000
#define THREAD_COUNT 4

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

// Atomic 변수

// Thread 변수

