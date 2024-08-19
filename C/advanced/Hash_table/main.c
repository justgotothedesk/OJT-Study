#include "define.h"
#include <stddef.h>

int main() {
	struct hash_table table[HASH_SIZE];

	for (int i = 0; i < HASH_SIZE; i++) {
		table[i].head = NULL;
		table[i].list_entry = 0;
	}

	read_csv_and_insert("hash.csv", table);
	print_hash_table(table);
	free_hash_table(table);

	return 0;
}
