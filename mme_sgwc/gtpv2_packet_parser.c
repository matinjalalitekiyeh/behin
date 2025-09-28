#include "gtpv2_packet_parser.h"


struct gtpv2_header {
    uint8_t flags;
    uint8_t message_type;
    uint16_t message_length;
    uint32_t teid;
    uint32_t sequence_number;
} __attribute__((packed));

struct gtpv2_ie {
    uint16_t type;
    uint16_t length;
    uint8_t *value;
} __attribute__((packed));

void parse_all_ies_recursive(const uint8_t *data, int length) {

    printf("Total IEs length: %d\n", length);

       const uint8_t *current = data;
       const uint8_t *end = data + length;

       while (current < end) {
           // Check if we have enough data for IE header (4 bytes: 1 type + 2 length + 1 spare)
           if (current + 4 > end) {
               printf("Not enough data for IE header. Remaining: %ld bytes\n", (long)(end - current));
               break;
           }

           // Parse IE header - GTPv2 IE format: 1 byte type, 2 bytes length, 1 byte spare
           uint8_t ie_type = current[0];
           uint16_t ie_length = (current[1] << 8) | current[2];
           // current[3] is spare byte (usually 0)

           printf("IE Type: 0x%02X, Length: %d", ie_type, ie_length);

           // Check if we have enough data for the IE value
           if (current + 4 + ie_length > end) {
               printf(" - INCOMPLETE (needs %d, has %ld)\n", ie_length, (long)(end - current - 4));
               break;
           }

           // Create IE structure
           struct gtpv2_ie ie;
           ie.type = ie_type;
           ie.length = ie_length;
           ie.value = (uint8_t *)(current + 4);

           printf(" - Value pointer: %p\n", (void*)ie.value);

           // Print first few bytes of value
           if (ie.length > 0) {
               printf("  First few bytes: ");
               for (int i = 0; i < (ie.length < 8 ? ie.length : 8); i++) {
                   printf("%02X ", ie.value[i]);
               }
               printf("\n");
           }

           // Move to next IE (4 byte header + IE value length)
           current += 4 + ie_length;
       }

       printf("Parsed %ld bytes out of %d total\n", (long)(current - data), length);

}

int is_gtpv2_traffic(const unsigned char *packet, int length, bool* is_retrans) {
    *is_retrans = false;

    // Minimum size check for Ethernet + IP + UDP + GTPv2 header
    if (length < (int)(sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct gtpv2_header))) {
        return 0;
    }

    // Check Ethernet type (IP)
    struct ethhdr *eth = (struct ethhdr *)packet;
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        return 0;
    }

    // Check IP protocol (UDP)
    struct iphdr *ip = (struct iphdr *)(packet + sizeof(struct ethhdr));
    if (ip->protocol != IPPROTO_UDP) {
        return 0;
    }

    // Check IP header length
    int ip_header_len = ip->ihl * 4;
    if (length < (int)(sizeof(struct ethhdr) + ip_header_len + sizeof(struct udphdr))) {
        return 0;
    }

    // Get UDP header
    struct udphdr *udp = (struct udphdr *)(packet + sizeof(struct ethhdr) + ip_header_len);

    // Check if it's GTPv2 port (2123)
    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->daddr), dst_ip, INET_ADDRSTRLEN);

    if((strcmp(src_ip, "127.0.0.2") == 0) && (strcmp(dst_ip, "127.0.0.3") == 0) ||
            (strcmp(src_ip, "127.0.0.3") == 0) && (strcmp(dst_ip, "127.0.0.2") == 0)) {
    } else {
        return 0;
    }

    if (ntohs(udp->dest) != GTPV2_PORT && ntohs(udp->source) != GTPV2_PORT) {
        return 0;
    }

    // Calculate UDP payload length
    int udp_header_len = sizeof(struct udphdr);
    int udp_payload_len = ntohs(udp->len) - udp_header_len;

    // Check if we have enough data for GTPv2 header
    if (udp_payload_len < (int)sizeof(struct gtpv2_header)) {
        return 0;
    }

    // Get GTPv2 header
    struct gtpv2_header *gtp = (struct gtpv2_header *)(packet + sizeof(struct ethhdr) + ip_header_len + udp_header_len);

    // Parse flags
    uint8_t version = (gtp->flags >> 5) & 0x07;
    uint8_t piggyback = (gtp->flags >> 4) & 0x01;
    uint8_t teid_flag = (gtp->flags >> 3) & 0x01;

    // Message Type
    printf("=== GTPv2 Message === ");
    printf("Message Type: %d (0x%02X)\n", gtp->message_type, gtp->message_type);

    // Message Length
    uint16_t msg_len = ntohs(gtp->message_length);
//    printf("Message Length: %d bytes\n", msg_len);

    // Calculate available data for IEs
    int gtp_header_size = sizeof(struct gtpv2_header);
    int ies_length = udp_payload_len - gtp_header_size;


    if (ies_length > 0) {
//        printf("len: %d\n", msg_len);
        const unsigned char *ies_data = (const unsigned char *)(packet + sizeof(struct ethhdr) + ip_header_len + udp_header_len + gtp_header_size);

//        printf("=== Information Elements ===\n");
        parse_all_ies_recursive(ies_data, ies_length);
    }

    printf("============================\n");

    return 1;
}
