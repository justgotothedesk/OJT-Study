#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct hash_table {
    struct linked_list *head;
    int list_entry;
};

struct linked_list {
    struct linked_list *next;
    char *data;
};

int main() {
    srand(time(NULL));
    int hash_size = 31;
    char *key[50];
    FILE *fp = fopen("C:\\실무\\build\\hash.csv", "r");
    if (fp == NULL) {
        perror("There is no file\n");

        return 0;
    }

    char line[1024];
    int cursor = 0;
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        while (token) {
            if (cursor >= 50) {
                perror("Too many keys\n");
                fclose(fp);

                return 0;
            }
            key[cursor] = strdup(token);
            if (key[cursor] == NULL) {
                perror("Many allocation failed\n");
                fclose(fp);

                return 0;
            }
            cursor += 1;
            token = strtok(NULL, ",");
        }
    }

    fclose(fp);

    struct hash_table table[hash_size];
    for (int i = 0; i < hash_size; i++) {
        table->head = NULL;
        table->list_entry = 0;
    }

    for (int i = 0; i < cursor; i++) {
        int index = rand()%hash_size;
        struct linked_list *new_node = (struct linked_list *)malloc(sizeof(struct linked_list));
        if (new_node == NULL) {
            perror("Memory allocation failed\n");
            
            return 0;
        }

        new_node->data = key[i];
        new_node->next = NULL;

        if (table[index].head == NULL) {
            table[index].head = new_node;
        } else {
            struct linked_list *current = table[index].head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }

        table[index].list_entry++;
    }

    for (int i = 0; i < hash_size; i++) {
        printf("Index: %d ", i);
        struct linked_list *current = table[i].head;
        while (current) {
            printf("%s -> ", current->data);
            current = current->next;
        }
        printf("\n");
    }

    for (int i = 0; i < hash_size; i++) {
        struct linked_list *current = table[i].head;
        while (current) {
            struct linked_list *temp = current;
            current = current->next;
            free(temp->data);
            free(temp);
        }
    }

    return 0;
}