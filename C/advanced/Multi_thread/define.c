#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "define.h"
#include <time.h>

pthread_mutex_t print_mutex;
int repeat_num;
int thread_num;
char thread_name[256][256];

__thread hash_table tls_table[HASH_SIZE];

const char *status_to_string(ThreadStatus status) {
    switch (status) {
        case INIT:
            return "INIT";
        case RUNNING:
            return "RUNNING";
        case DONE:
            return "DONE";
        case SLEEP:
            return "SLEEP";
        default:
            return "UNKNOWN";
    }
}

char* extract_value(const char *json_string, const char *key, char *buffer) {
    char *key_position = strstr(json_string, key);
    if (!key_position) {
        return NULL;
    }

    char *colon = strchr(key_position, ':');
    if (!colon) {
        return NULL;
    }

    char *value_start = colon+1;
    while (*value_start == ' ' || *value_start == '\"' || *value_start == '\n') {
        value_start++;
    }

    char *value_end;
    if (*value_start == '[') {
        value_end = strchr(value_start, ']')+1;
    } else {
        value_end = value_start;
        while (*value_end != ' ' && *value_end != '\"' && *value_end != '\n' && *value_end != ',' && *value_end != '}' && *value_end != ']') {
             value_end++;
        }
    }

    strncpy(buffer, value_start, value_end - value_start);
    buffer[value_end - value_start] = '\0';

    return buffer;
}

int parse_json(const char *json_string) {
    char buffer[256];

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
                        char *name_ptr = buffer+1;
                        char name_buffer[256];
                        int name_index = 0;

                        while ((name_ptr = strtok(name_ptr, ",]"))) {
                            char *name_start = name_ptr;
                            while (*name_start == ' ' || *name_start == '\"') {
                                name_start++;
                            }
                            char *name_end = name_start+strlen(name_start)-1;
                            while (*name_end == ' ' || *name_end == '\"') {
                                name_end--;
                            }
                            *(name_end+1) ='\0';

                            printf("sub_thread name: %s\n", name_start);
                            strcpy(thread_name[name_index], name_start);

                            name_index++;
                            name_ptr = NULL;
                        }
                    }
                }
                ptr++;
            }
            printf("=================\n\n");
        }
    }

    return 1;
}

int hash_function(const char *key) {
    int hash = 0;
    while (*key) {
        hash = (hash * 31 + *key) % HASH_SIZE;
        key++;
    }

    return hash;
}

linked_list *create_node(const char *key) {
    linked_list *node = (linked_list *)malloc(sizeof(linked_list));
    if (node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->key = strdup(key);
    if (node->key == NULL) {
        perror("Memory allocation failed");
        free(node);
        exit(EXIT_FAILURE);
    }
    node->next = NULL;

    return node;
}

void insert_to_table(hash_table *table, const char *key) {
    int index = random_index();
    linked_list *new_node = create_node(key);
    new_node->next = table[index].head;
    table[index].head = new_node;
    table[index].list_entry++;
}

void read_csv_and_insert(const char *filename, hash_table *table) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("No file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
	char *rest = line;
        char *token = strtok_r(line, ", \n", &rest);
        while (token) {
            insert_to_table(table, token);
            token = strtok_r(NULL, ", \n", &rest);
        }
    }

    fclose(fp);
}

void print_hash_table(hash_table *table, ThreadInfo *thread) {
    pthread_mutex_lock(&print_mutex);
    printf("----- Thread %s's hash table -----\n", thread->name);
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("Hash index: %d ", i);
        linked_list *current = table[i].head;
        while (current) {
            printf("[key: %s] -> ", current->key);
            current = current->next;
        }
        printf("NULL\n");
    }
    pthread_mutex_unlock(&print_mutex);
}

void free_hash_table(hash_table *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        linked_list *current = table[i].head;
        while (current) {
            linked_list *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
}

void *thread_func(void *arg) {
    ThreadInfo *thread_info = (ThreadInfo *)arg;

    thread_info->status = INIT;
    printf("Thread %s's status: %s\n", thread_info->name, status_to_string(thread_info->status));

    for (int i = 0; i < HASH_SIZE; i++) {
	    tls_table[i].head = NULL;
	    tls_table[i].list_entry = 0;
    }

    read_csv_and_insert("hash.csv", tls_table);
    printf("\n");

    sleep(1);

    thread_info->status = RUNNING;
    printf("Thread %s's status: %s\n", thread_info->name, status_to_string(thread_info->status));
    printf("\n");

    sleep(1);

    thread_info->status = SLEEP;
    printf("Thread %s's status: %s\n", thread_info->name, status_to_string(thread_info->status));
    print_hash_table(tls_table, thread_info);
    printf("\n");

    sleep(1);

    thread_info->status = DONE;
    printf("Thread %s's status: %s\n", thread_info->name, status_to_string(thread_info->status));
    print_hash_table(tls_table, thread_info);
    printf("\n");

    free_hash_table(tls_table);

    return NULL;
}

int random_index() {
	return rand()%HASH_SIZE;
}
