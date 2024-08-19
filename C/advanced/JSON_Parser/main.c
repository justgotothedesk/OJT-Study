#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "parser.h"

int thread_num = 0;
int repeat_num = 0;
char thread_name[256][256];

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

    pthread_t threads[256];
    int thread_indices[256];
    for (int i = 0; i < thread_num; i++) {
        thread_indices[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_indices[i]);
    }

    for (int i = 0; i < thread_num; i++) {
        pthread_join(threads[i], NULL);
    }

    free(json_string);

    return 0;
}
