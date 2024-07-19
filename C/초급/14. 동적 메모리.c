#include <stdio.h>
#include <stdlib.h>

size_t total_allocated_memory = 0;
size_t total_freed_memory = 0;

int* memory(int size) {
    int* arr = (int*)malloc(sizeof(int)*size);
    if (arr != NULL) {
        total_allocated_memory += sizeof(int)*size;
    }
    printf("total allocated memory: %dKB, used memory: %dKB, freed memory: %dKB\n",
    total_allocated_memory/1024,
    (total_allocated_memory-total_freed_memory)/1024,
    total_freed_memory/1024);

    return arr;
}

void custom_free(int *arr, int size) {
    if (arr != NULL) {
        free(arr);
        total_freed_memory += sizeof(int)*size;
    }
    printf("total allocated memory: %dKB, used memory: %dKB, freed memory: %dKB\n",
    total_allocated_memory/1024,
    (total_allocated_memory-total_freed_memory)/1024,
    total_freed_memory/1024);
}

int main() {
    printf("Col: ");
    int col;
    scanf("%d", &col);

    int *arr = memory(col);
    if (arr == NULL) {
        perror("error");

        return 0;
    }

    custom_free(arr, col);

    return 0;
}