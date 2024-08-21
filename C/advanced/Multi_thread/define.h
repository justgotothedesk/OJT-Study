#include <stdio.h>
#include <pthread.h>

#define HASH_SIZE 31

typedef struct linked_list {
	struct linked_list *next;
	char *key;
} linked_list;

typedef struct {
	linked_list *head;
	int list_entry;
} hash_table;

typedef enum {
	INIT,
	RUNNING,
	DONE,
	SLEEP
} ThreadStatus;

typedef struct {
	pthread_t thread_id;
	hash_table table[HASH_SIZE];
	char name[256];
	ThreadStatus status;
} ThreadInfo;

extern pthread_mutex_t print_mutex;
extern int repeat_num;
extern int thread_num;
extern char thread_name[256][256];

const char *status_to_string(ThreadStatus status);
char *extract_value(const char *json_string, const char *key, char *buffer);
int parse_json(const char *json_string);
int hash_function(const char *key);
linked_list *create_node(const char *key);
void insert_to_table(hash_table *table, const char *key);
void read_csv_and_insert(const char *filename, hash_table *table);
void print_hash_table(hash_table *table, ThreadInfo *thread);
void free_hash_table(hash_table *table);
void *thread_func(void *arg);
int random_index();
