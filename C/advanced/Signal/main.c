#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

int thread_num = 0;
int repeat_num = 0;
char thread_name[256][256];
const char *filename = "thread.json";

int main() {
	signal(SIGINT, print_json_and_exit);
	signal(SIGUSR1, reload_and_print_json);
	parse_json(filename);

	int count = 0;

	while (1) {
		printf("\n---------------------\n");
		printf("Thread is running %ds\n", count);
		printf("Thread # %d, repeat # %d\n", thread_num, repeat_num);
		printf("---------------------\n");
		count++;
		sleep(1);
	}

	return 0;
}


		
