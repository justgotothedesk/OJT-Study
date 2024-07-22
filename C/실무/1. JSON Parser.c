#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    char *value_end = value_start;
    while (*value_end != ' ' && *value_end != '\"' && *value_end != '\n' && *value_end != ',' && *value_end != '}' && *value_end != ']') {
        value_end++;
    }

    strncpy(buffer, value_start, value_end - value_start);
    buffer[value_end - value_start] = '\0';

    return buffer;
}

int parse_json(const char *json_string) {
    char buffer[256];

    if (extract_value(json_string, "repeat", buffer)) {
        printf("repeat: %s\n", buffer);
    }

    if (extract_value(json_string, "thread_num", buffer)) {
        printf("thread_num: %s\n", buffer);
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
                if (!ptr) break;
                if (extract_value(ptr, "name", buffer)) {
                    printf("sub_thread name: %s\n", buffer);
                }
                ptr++;
            }
        }
    }

    return 1;
}

int main() {
    const char *filename = "C:\\실무\\build\\thread.json";
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

    free(json_string);
    return 0;
}

// {
//     "repeat": 10,
//     "thread_num": 1,
//     "thread": [
//         {
//             "name": "sub_thread"
//         }
//     ]
// }
