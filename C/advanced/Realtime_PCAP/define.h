#ifndef DEFINE_H
#define DEFINE_H

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define CAPTURE_DURATION 60

void int_handler(int dummy);
void generate_filename(char *buffer, size_t len);
void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet);
void start_packet_capture(const char *interface);

#endif
