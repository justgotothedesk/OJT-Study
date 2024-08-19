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
pthread_t threads[256];
int thread_indices[256];
int timer = 1;

int main() {
	signal(SIGINT, print_json_and_exit);
	signal(SIGUSR1, reload_and_print_json);
	parse_json(filename);

	for (int i = 0; i < thread_num; i++) {
		thread_indices[i] = i;
		pthread_create(&threads[i], NULL, thread_function, &thread_indices[i]);
	}

	for (int i = 0; i < thread_num; i++) {
		pthread_join(threads[i], NULL);
	}
	
	printf("\n");
	while (1) {
		printf("Waiting for SIGNAL ... %ds\n", timer);
		timer++;
		sleep(1);
	}

	return 0;
}


		
