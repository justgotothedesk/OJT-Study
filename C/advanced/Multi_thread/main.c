#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stddef.h>
#include "define.h"

int thread_num = 0;
int repeat_num = 0;
char thread_name[256][256];
pthread_mutex_t print_mutx = PTHREAD_MUTEX_INITIALIZER;

int main() {
    const char *filename = "thread.json";
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_string = (char *)malloc(length + 1);
    if (!json_string) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return 1;
    }

    fread(json_string, 1, length, file);
    json_string[length] = '\0';
    fclose(file);

    parse_json(json_string);

    ThreadInfo *threads = malloc(thread_num * sizeof(ThreadInfo));
    if (!threads) {
        fprintf(stderr, "Memory allocation error\n");
        free(json_string);
        return 1;
    }

    for (int i = 0; i < thread_num; i++) {
        for (int j = 0; j < HASH_SIZE; j++) {
            threads[i].table[j].head = NULL;
            threads[i].table[j].list_entry = 0;
        }
        strcpy(threads[i].name, thread_name[i]);
        threads[i].status = INIT;

        if (pthread_create(&threads[i].thread_id, NULL, thread_func, &threads[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < thread_num; i++) {
        pthread_join(threads[i].thread_id, NULL);
    }

    free(threads);
    pthread_mutex_destroy(&print_mutex);

    return 0;
}
