#include "define.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <interface>\n", argv[0]);

		return 1;
	}

	start_packet_capture(argv[1]);

	return 0;
}
