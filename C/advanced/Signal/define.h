extern int repeat_num;
extern int thread_num;
extern char thread_name[256][256];
extern const char *filename;

char *extract_value(const char *json_string, const char *key, char *buffer);
int parse_json(const char *json_string);
void print_json_and_exit(int sig);
void reload_and_print_json(int sig);
