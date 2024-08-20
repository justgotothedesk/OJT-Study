#include "define.h"

void print_network_info() {
	struct ifaddrs *ifaddr, *ifa;
	char ip[INET_ADDRSTRLEN];
	char netmask[INET_ADDRSTRLEN];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL) {
			continue;
		}

		if (ifa->ifa_addr->sa_family == AF_INET) {
			printf("Interface: %s\n", ifa->ifa_name);

			struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
			inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);
			printf("  IP address: %s\n", ip);

			struct sockaddr_in *netmask_addr = (struct sockaddr_in *)ifa->ifa_netmask;
			inet_ntop(AF_INET, &netmask_addr->sin_addr, netmask, INET_ADDRSTRLEN);
			printf("  Subnet Mask: %s\n", netmask);
		}
	}

	freeifaddrs(ifaddr);
}

void print_network_info2() {
    char *dev;  
    char *net;   
    char *mask;   
    int ret;       
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp;   
    bpf_u_int32 maskp; 
    struct in_addr addr;

    dev = pcap_lookupdev(errbuf);

    if(dev == NULL)
    {
        printf("%s\n",errbuf);
        exit(1);
    }

    printf("DEV: %s\n",dev);

    ret = pcap_lookupnet(dev,&netp,&maskp,errbuf);

    if(ret == -1)
    {
        printf("%s\n",errbuf);
        exit(1);
    }

    addr.s_addr = netp;
    net = inet_ntoa(addr);

    if(net == NULL)
    {
        perror("inet_ntoa");
        exit(1);
    }

    printf("NET: %s\n",net);

    addr.s_addr = maskp;
    mask = inet_ntoa(addr);
  
    if(mask == NULL)
    {
        perror("inet_ntoa");
        exit(1);
    }
  
    printf("MASK: %s\n",mask);
}

void parse_pcap(const char *filename) {
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle = pcap_open_offline(filename, errbuf);
	if (!handle) {
		perror("Error opening PCAP file\n");
		exit(EXIT_FAILURE);
	}

	struct pcap_pkthdr *header;
	const u_char *packet;
	int res;

	printf("Source IP: port -> Destination IP: port [Protocol]\n\n");
	
	while ((res = pcap_next_ex(handle, &header, &packet)) >= 0) {
		if (res == 0) {
			continue;
		}

		struct ether_header *eth = (struct ether_header *)packet;

		if (ntohs(eth->ether_type) != ETHERTYPE_IP) {
			printf("Not a IPv4 packet\n");
			continue;
		}

		struct ip *ip_hdr = (struct ip *)(packet+sizeof(struct ether_header));
		
		if (ip_hdr->ip_p == IPPROTO_TCP) {
			struct tcphdr *tcp_hdr = (struct tcphdr *)(packet+sizeof(struct ether_header)+(ip_hdr->ip_hl*4));
			
			char src_ip[INET_ADDRSTRLEN];
			char dst_ip[INET_ADDRSTRLEN];

			inet_ntop(AF_INET, &(ip_hdr->ip_src), src_ip, INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &(ip_hdr->ip_dst), dst_ip, INET_ADDRSTRLEN);

			printf("%s:%d   ->   %s:%d [TCP]\n",
					src_ip, ntohs(tcp_hdr->source),
					dst_ip, ntohs(tcp_hdr->dest));
		} else {
			printf("Non-TCP packet with protocal %d\n", ip_hdr->ip_p);
		}
	}

	pcap_close(handle);
}
