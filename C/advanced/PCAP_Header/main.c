#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "define.h"

int main(int argc, char *argv[]) {
	printf("\n");
	print_network_info();
	printf("\n");
	print_network_info2();

	printf("\n=================================================\n\n");

	char *filename = NULL;
	int opt;

	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
			case 'f':
				filename = optarg;
				break;
			default:
				printf("Usage: %s -f <pcap_file>\n", argv[0]);
				
				return EXIT_FAILURE;
		}
	}

	if (!filename) {
		printf("Usage: %s -f <pcap_file>\n", filename);

		return EXIT_FAILURE;
	}

	parse_pcap(filename);


	return 0;
}
