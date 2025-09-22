#ifndef GTPV2_PACKET_PARSER_H
#define GTPV2_PACKET_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include "socket_mdl.h"
#include "pcap_module.h"

#include <netinet/udp.h>

#define BUFFER_SIZE 65536
#define SNAP_LEN 1518
#define PCAP_MAGIC 0xa1b2c3d4

// Configuration - CHANGE THESE
#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.0.3"
#define GTPV2_PORT 2123


struct gtpv2_header {
    uint8_t flags;
    uint8_t message_type;
    uint16_t message_length;
    uint32_t teid;
    uint32_t sequence_number;
};


int is_gtpv2_traffic(const unsigned char *packet, int length);
//void print_gtpv2_info(const struct gtpv2_header *gtp, const struct iphdr *ip, const struct udphdr *udp);

#endif /* GTPV2_PACKET_PARSER_H */
