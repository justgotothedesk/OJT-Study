#ifndef DEFINE_H
#define DEFINE_H

#define HASH_SIZE 31

struct hash_table {
	struct linked_list *head;
	int list_entry;
};

struct linked_list {
	struct linked_list *next;
	char *key;
};

int hash_function(const char *key);
int random_index();
struct linked_list *create_node(const char *key);
void insert_to_table(struct hash_table *table, const char *key);
void read_csv_and_insert(const char *filename, struct hash_table *table);
void print_hash_table(struct hash_table *table);
void free_hash_table(struct hash_table *table);

#endif
