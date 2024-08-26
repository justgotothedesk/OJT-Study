#ifndef __PCAP_HEADER_H__
#define __PCAP_HEADER_H__

struct sd_pcap_file_header {
	u_int32_t magic;
	u_int16_t version_major;
	u_int16_t version_minor;
	u_int32_t thiszone;
	u_int32_t sigfigs;
	u_int32_t snaplen;
	u_int32_t linktype;
};

int file_write_pcap_file_header(void* fp);

#endif
