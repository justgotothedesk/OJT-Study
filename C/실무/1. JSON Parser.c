#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

int repeat_num = 0;
int thread_num = 0;
char thread_name[256][256];

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
                            printf("=================\n\n");

                            strcpy(thread_name[name_index], name_start);
                            name_index++;
                            
                            name_ptr = NULL;
                        }
                    }
                }
                ptr++;
            }
        }
    }

    return 1;
}

void genearte_random_word(char *word, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int charset_size = sizeof(charset)-1;

    for (int i = 0; i < length; i++) {
        int random_index = rand()%charset_size;
        word[i] = charset[random_index];
    }
    word[length] = '\0';
}

void* thread_function(void* arg) {
    int length = 5;
    char random_word[length+1];
    int index = *(int*)arg;
    
    for (int i = 0; i < repeat_num; i++) {
        genearte_random_word(random_word, length);
        printf("Thread %s running, random word: %s\n", thread_name[index], random_word);
    }

    return NULL;
}

int main() {
    srand(time(NULL));
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