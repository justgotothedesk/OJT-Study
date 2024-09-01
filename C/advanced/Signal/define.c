#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>

char* extract_value(const char *json_string, const char *key, char *buffer) {
    char *key_position = strstr(json_string, key);
    if (!key_position) {
        return NULL;
    }

    char *colon = strchr(key_position, ':');
    if (!colon) {
        return NULL;
    }

    char *value_start = colon + 1;
    while (*value_start == ' ' || *value_start == '\"' || *value_start == '\n') {
        value_start++;
    }

    char *value_end;
    if (*value_start == '[') {
        value_end = strchr(value_start, ']') + 1;
    } else {
        value_end = value_start;
        while (*value_end != ' ' && *value_end != '\"' && *value_end != '\n' &&
               *value_end != ',' && *value_end != '}' && *value_end != ']') {
            value_end++;
        }
    }

    strncpy(buffer, value_start, value_end - value_start);
    buffer[value_end - value_start] = '\0';

    return buffer;
}

int parse_json(const char *filename) {
    char buffer[256];
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");

        return 0;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_string = (char *)malloc(length + 1);
    if (!json_string) {
        perror("Memory allocation error");
        fclose(file);

        return 0;
    }

    fread(json_string, 1, length, file);
    json_string[length] = '\0';
    fclose(file);

    if (extract_value(json_string, "repeat", buffer)) {
        printf("repeat: %s\n", buffer);
        repeat_num = atoi(buffer);
    }

    if (extract_value(json_string, "thread_num", buffer)) {
        printf("thread_num: %s\n", buffer);
        thread_num = atoi(buffer);
    }

    const char *thread_array = strstr(json_string, "\"thread\"");
    if (thread_array) {
        thread_array = strchr(thread_array, '[');
        if (thread_array) {
            int thread_count = 0;
            const char *ptr = thread_array;
            while ((ptr = strchr(ptr, '{')) != NULL) {
                thread_count++;
                ptr++;
            }
            printf("thread array size: %d\n", thread_count);

            ptr = thread_array;
            for (int i = 0; i < thread_count; i++) {
                ptr = strchr(ptr, '{');
                if (!ptr) {
                    break;
                }
                if (extract_value(ptr, "name", buffer)) {
                    if (buffer[0] == '[') {
                        char *name_ptr = buffer + 1;
                        int name_index = 0;
			char *rest;

                        while ((name_ptr = strtok_r(name_ptr, ",]", &rest))) {
                            char *name_start = name_ptr;
                            while (*name_start == ' ' || *name_start == '\"') {
                                name_start++;
                            }

                            char *name_end = name_start + strlen(name_start) - 1;
                            while (*name_end == ' ' || *name_end == '\"') {
                                name_end--;
                            }

                            *(name_end + 1) = '\0';

                            printf("sub_thread name: %s\n", name_start);

                            strcpy(thread_name[name_index], name_start);
                            name_index++;

                            name_ptr = NULL;
                        }
                    }
                }
                ptr++;
            }
            printf("===========================\n\n");
        }
    }

    free(json_string); 
    return 1; 
}

void print_json_and_exit(int sig) {
    if (sig == SIGINT) {
        printf("\nReceived SIGINT. Now printing JSON content ...\n");
	timer = 1;

        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Error opening file");
            return;
        }

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }

        fclose(file);
	printf("===========================\n\n");

	int timer = 3;
        
	while (timer) {
		printf("%d ...\n", timer);
		sleep(1);
		timer--;
	}

        exit(0);
    }
}

void reload_and_print_json(int sig) {
    if (sig == SIGUSR1) {
        printf("\nReceived SIGUSR1. Now reloading and printing JSON content ...\n");
	timer = 1;

        parse_json(filename);

        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Error opening file");
            return;
        }

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }

        fclose(file);
	printf("===========================\n\n");

	for (int i = 0; i < thread_num; i++) {
		pthread_create(&threads[i], NULL, thread_function, &thread_indices[i]);
	}

	for (int i = 0; i < thread_num; i++) {
		pthread_join(threads[i], NULL);
	}
    }

}

void* thread_function(void* arg) {
	int index = *(int*)arg;
	for (int i = 0; i < repeat_num; i++) {
		printf("Thread %s is running, %d times\n", thread_name[index], i+1);
		sleep(1);
	}

	return;
}

