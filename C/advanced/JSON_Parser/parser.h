#include <stdio.h>

extern int repeat_num;
extern int thread_num;
extern char thread_name[256][256];

char *extract_value(const char *json_string, const char *key, char *buffer);
int parse_json(const char *json_string);
void generate_random_word(char *word, int length);
void *thread_function(void* arg);
