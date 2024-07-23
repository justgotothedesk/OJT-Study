#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <json-c/json.h>

#define HASH_SIZE 31

struct hash_table {
    struct linked_list *head;
    int list_entry;
};

struct linked_list {
    struct linked_list *next;
    char *data;
};

enum thread_status {
    INIT, RUNNING, DONE, SLEEP
};

struct thread_info {
    pthread_t thread_id;
    char *name;
    char *filename;
    struct hash_table *table;
    enum thread_status status;
};

void *thread_func(void *arg) {
    struct thread_info *tinfo = arg;
    tinfo->status = INIT;
    printf("%s: INIT\n", tinfo->name);
    SLEEP(1);

    srand(time(NULL)^pthread_self());

    tinfo->table = malloc(sizeof(struct hash_table)*HASH_SIZE);
    for (int i = 0; i < HASH_SIZE; i++) {
        tinfo->table[i].head = NULL;
        tinfo->table[i].list_entry = 0;
    }

    tinfo->status = RUNNING;
    printf("%s: RUNNING\n", tinfo->name);
    SLEEP(1);

    FILE *fp = fopen(tinfo->filename, "r");
    if (fp == NULL) {
        perror("There is no file");
        tinfo->status = DONE;
        
        return 0;
    }

    char line[1024];
    int cursor = 0;
    char *key[50];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        while (token) {
            if (cursor >= 50) {
                perror("Too many keys\n");
                fclose(fp);
                tinfo->status = DONE;
                return NULL;
            }
            key[cursor] = strdup(token);
            if (key[cursor] == NULL) {
                perror("Memory allocation failed\n");
                fclose(fp);
                tinfo->status = DONE;
                return NULL;
            }
            cursor += 1;
            token = strtok(NULL, ",");
        }
    }
    fclose(fp);

    for (int i = 0; i < cursor; i++) {
        int index = rand() % HASH_SIZE;
        struct linked_list *new_node = (struct linked_list *)malloc(sizeof(struct linked_list));
        if (new_node == NULL) {
            perror("Memory allocation failed\n");
            tinfo->status = DONE;
            return NULL;
        }

        new_node->data = key[i];
        new_node->next = NULL;

        if (tinfo->table[index].head == NULL) {
            tinfo->table[index].head = new_node;
        } else {
            struct linked_list *current = tinfo->table[index].head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }

        tinfo->table[index].list_entry++;
    }

    tinfo->status = SLEEP;
    printf("%s: SLEEP\n", tinfo->name);
    sleep(2);

    tinfo->status = DONE;
    printf("%s: DONE\n", tinfo->name);

    return NULL;
}

int main() {
    FILE *fp = fopen("C\\실무\\build\\thread.json", "r");
    if (fp == NULL) {
        perror("There is no file");

        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *json_str = malloc(fsize+1);
    fread(json_str, 1, fsize, fp);
    fclose(fp);
    json_str[fsize] = 0;

    struct json_object *parsed_json;
    struct json_object *repeat;
    struct json_object *thread_num;
    struct json_object *threads;
    struct json_object *thread;
    struct json_object *name;

    parsed_json = json_tokener_parse(json_str);
    json_object_object_get_ex(parsed_json, "repeat", &repeat);
    json_object_object_get_ex(parsed_json, "thread_num", &thread_num);
    json_object_object_get_ex(parsed_json, "thread", &threads);

    int num_threads = json_object_get_int(thread_num);

    struct thread_info tinfo[num_threads];

    char filenames[num_threads][20];
    for (int i = 0; i < num_threads; i++) {
        sprintf(filenames[i], "hash%d.csv", i+1)    ;
        thread = json_object_array_get_idx(threads, i);
        json_object_get_ex(thread, "name", &name);

        tinfo[i].name = strdup(json_object_get_string(name));
        tinfo[i].filename = filenames[i];
        tinfo[i].status = INIT;

        pthread_create(&tinfo[i].thread_id, NULL, thread_func, &tinfo[i]);
    }

    int all_done = 0;
    while (!all_done) {
        all_done = 1;
        for (int i = 0; i < num_threads; i++) {
            printf("%s: ", tinfo[i].name);
            switch (tinfo[i].status)
            {
                case INIT:
                    printf("INIT\n");
                    break;
                case RUNNING:
                    printf("RUNNING\n");
                    break;
                case SLEEP:
                    printf("SLEEP\n");
                    break;
                case DONE:
                    printf("DONE\n");
                    break;            
                default:
                    break;
            }
            if (tinfo[i].status != DONE) {
                all_done = 0;
            }
        }
        SLEEP(1);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(tinfo[i].thread_id, NULL);
        printf("%s hash table:\n", tinfo[i].name);
    }

    free(json_str);
    json_object_put(parsed_json);

    return 0;
}