#include "define.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


int hash_function(const char *key) {
    int hash = 0;
    while (*key) {
        hash = (hash*31+*key)%HASH_SIZE;
        key++;
    }

    return hash;
}

struct linked_list *create_node(const char *key) {
    struct linked_list *node = (struct linked_list *)malloc(sizeof(struct linked_list));
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

void insert_to_table(struct hash_table *table, const char *key) {
    int index = hash_function(key);
    struct linked_list *new_node = create_node(key);
    new_node->next = table[index].head;
    table[index].head = new_node;
    table[index].list_entry++;
}

void read_csv_and_insert(const char *filename, struct hash_table *table) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("No file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ", \n");
        while (token) {
            insert_to_table(table, token);
            token = strtok(NULL, ", \n");
        }
    }

    fclose(fp);
}

void print_hash_table(struct hash_table *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("Hash index: %d ", i);
        struct linked_list *current = table[i].head;
        while (current) {
            printf("[key: %s] -> ", current->key);
            current = current->next;
        }
        printf("NULL\n");
    }
}

void free_hash_table(struct hash_table *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        struct linked_list *current = table[i].head;
        while (current) {
            struct linked_list *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
}


