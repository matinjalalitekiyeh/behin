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
    uint8_t value[0];
} __attribute__((packed));

void parse_all_ies_recursive(const uint8_t *data, int length, int depth) {
    int offset = 0;

    while (offset + 4 <= length) {
        struct gtpv2_ie *ie = (struct gtpv2_ie *)(data + offset);

        // Read as little-endian
        uint16_t ie_type = ((uint8_t*)&ie->type)[0] | (((uint8_t*)&ie->type)[1] << 8);
        uint16_t ie_length = ((uint8_t*)&ie->length)[0] | (((uint8_t*)&ie->length)[1] << 8);

        // Print IE type with indentation
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("IE: %d\n", ie_type);

        // Recursively parse container IEs
        if (ie_length > 0 && offset + 4 + ie_length <= length) {
            parse_all_ies_recursive(ie->value, ie_length, depth + 1);
        }

        // Move to next IE
        offset += 4 + ie_length;

        // Align to 4-byte boundary
        while (offset % 4 != 0 && offset < length) {
            offset++;
        }
    }
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
    printf("=== GTPv2 Message ===\n");
    printf("Message Type: %d (0x%02X)\n", gtp->message_type, gtp->message_type);

    // Message Length
    uint16_t msg_len = ntohs(gtp->message_length);
    printf("Message Length: %d bytes\n", msg_len);

    // Calculate available data for IEs
    int gtp_header_size = sizeof(struct gtpv2_header);
    int ies_length = udp_payload_len - gtp_header_size;

    if (ies_length > 0) {
        const unsigned char *ies_data = (const unsigned char *)(packet + sizeof(struct ethhdr) + ip_header_len + udp_header_len + gtp_header_size);

        printf("=== Information Elements ===\n");
        parse_all_ies_recursive(ies_data, ies_length, 0);
    }

    printf("============================\n");

    return 1;
}
