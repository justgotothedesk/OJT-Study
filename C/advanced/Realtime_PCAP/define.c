#include "define.h"
#include "pcap_header.h"

volatile int keep_running = 1;

void int_handler(int dummy) {
	keep_running = 0;
}

void generate_filename(char *buffer, size_t len) {
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	strftime(buffer, len, "%Y_%m_%d_%H_%M.cap", t);
}

void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
	FILE *fp = (FILE *)user;
	fwrite(packet, pkthdr->caplen, 1, fp);
}

void start_packet_capture(const char *interface) {
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	char filename[64];
	FILE *fp;

	handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
	if (!handle) {
		fprintf(stderr, "Can't open %s interface, because %s\n", interface, errbuf);
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, int_handler);

	while (keep_running) {
		generate_filename(filename, sizeof(filename));

		fp = fopen(filename, "wb");

		if (!fp) {
			perror("Error opening file");
			pcap_close(handle);
			exit(EXIT_FAILURE);
		}

		file_write_pcap_file_header(fp);

		time_t start_time = time(NULL);
		while (difftime(time(NULL), start_time) < CAPTURE_DURATION && keep_running) {
			pcap_dispatch(handle, 0, packet_handler, (u_char *)fp);
		}

		printf("Successfully packet detected\n");

		fclose(fp);
	}

	pcap_close(fp);
}
