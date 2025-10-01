#ifndef GTPV2_PACKET_PARSER_H
#define GTPV2_PACKET_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include "sniff_sock.h"
#include "pcap_module.h"

#include <netinet/udp.h>

#define BUFFER_SIZE 65536
#define SNAP_LEN 1518
#define PCAP_MAGIC 0xa1b2c3d4

// Configuration - CHANGE THESE
#define TARGET_CLIENT_IP "127.0.0.4"
#define TARGET_SERVER_IP "127.0.0.7"
#define PFCP_PORT 8805




int is_gtpv2_traffic(const unsigned char *packet, int length, bool *is_retrans);
//void print_gtpv2_info(const struct gtpv2_header *gtp, const struct iphdr *ip, const struct udphdr *udp);

#endif /* GTPV2_PACKET_PARSER_H */
