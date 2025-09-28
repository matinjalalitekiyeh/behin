#include "gtpv2_packet_parser.h"

struct gtpv2_header {
    uint8_t flags;
    uint8_t message_type;
    uint16_t message_length;
    uint32_t teid; // tunnel_endpoint_identifier
    uint32_t sequence_number;
} __attribute__((packed));

struct gtpv2_ie {
    uint16_t type;
    uint16_t length;
    uint8_t value[0]; // flexible array
} __attribute__((packed));

// F-TEID structure (Fully Qualified Tunnel Endpoint Identifier)
struct f_teid_ie {
    uint8_t interface_type;
    uint8_t teid_gre_key[4];
    uint32_t ipv4_address;
    uint8_t ipv6_address[16];
} __attribute__((packed));

void parse_f_teid_ie(struct gtpv2_ie *ie, uint16_t ie_length) {
    printf("=== F-TEID (Fully Qualified Tunnel Endpoint Identifier) ===\n");

    if (ie_length < 1) {
        printf("Error: F-TEID IE too short\n");
        return;
    }

    uint8_t *data = ie->value;
    int offset = 0;

    // Parse flags (first byte)
    uint8_t flags = data[offset++];
    uint8_t v4_present = (flags >> 0) & 0x01;  // Bit 0: IPv4 address present
    uint8_t v6_present = (flags >> 1) & 0x01;  // Bit 1: IPv6 address present
    uint8_t interface_type = (flags >> 2) & 0x1F; // Bits 2-6: Interface type

    printf("Flags: 0x%02X\n", flags);
    printf("  IPv4 Present: %d\n", v4_present);
    printf("  IPv6 Present: %d\n", v6_present);
    printf("  Interface Type: %d (", interface_type);

    // Interface type mapping
    switch (interface_type) {
        case 0: printf("S1-U eNodeB GTP-U interface"); break;
        case 1: printf("S1-U SGW GTP-U interface"); break;
        case 2: printf("S12 RNC GTP-U interface"); break;
        case 3: printf("S12 SGW GTP-U interface"); break;
        case 4: printf("S5/S8 SGW GTP-U interface"); break;
        case 5: printf("S5/S8 PGW GTP-U interface"); break;
        case 6: printf("S5/S8 SGW GTP-C interface"); break;
        case 7: printf("S5/S8 PGW GTP-C interface"); break;
        case 10: printf("S11 MME GTP-C interface"); break;
        case 11: printf("S11/S4 SGW GTP-C interface"); break;
        case 14: printf("Gn/Gp SGW GTP-C interface"); break;
        case 15: printf("Gn/Gp GGSN GTP-U interface"); break;
        case 16: printf("Gn/Gp GGSN GTP-C interface"); break;
        case 20: printf("S2b ePDG GTP-C interface"); break;
        case 21: printf("S2b-U ePDG GTP-U interface"); break;
        case 22: printf("S2b PGW GTP-C interface"); break;
        case 23: printf("S2b-U PGW GTP-U interface"); break;
        case 24: printf("S5 SGW GTP-U interface"); break;
        case 25: printf("S5 PGW GTP-U interface"); break;
        default: printf("Unknown interface type");
    }
    printf(")\n");

    // Parse TEID/GRE Key (always 4 bytes)
    if (offset + 4 <= ie_length) {
        uint32_t teid = (data[offset] << 24) | (data[offset+1] << 16) |
                       (data[offset+2] << 8) | data[offset+3];
        printf("TEID/GRE Key: %u (0x%08X)\n", teid, teid);
        offset += 4;
    }

    // Parse IPv4 address if present
    if (v4_present && offset + 4 <= ie_length) {
        struct in_addr ipv4_addr;
        memcpy(&ipv4_addr, data + offset, 4);
        printf("IPv4 Address: %s\n", inet_ntoa(ipv4_addr));
        offset += 4;
    }

    // Parse IPv6 address if present
    if (v6_present && offset + 16 <= ie_length) {
        char ipv6_str[INET6_ADDRSTRLEN];
        if (inet_ntop(AF_INET6, data + offset, ipv6_str, INET6_ADDRSTRLEN)) {
            printf("IPv6 Address: %s\n", ipv6_str);
        }
        offset += 16;
    }

    printf("Total F-TEID IE Length: %d bytes\n", ie_length);
    printf("Parsed bytes: %d\n", offset);
    printf("============================\n");
}

void parse_gtpv2_ies(const uint8_t *data, int length, uint32_t sequence_number) {
    int offset = 0;
    int ie_count = 0;
    int f_teid_found = 0;

    printf("=== GTPv2 Information Elements ===\n");
    printf("Sequence Number: %u\n", sequence_number);

    while (offset < length) {
        // Check if we have enough data for IE header
        if (offset + 4 > length) {
            printf("Incomplete IE header at offset %d\n", offset);
            break;
        }

        struct gtpv2_ie *ie = (struct gtpv2_ie *)(data + offset);
        uint16_t ie_type = ntohs(ie->type);
        uint16_t ie_length = ntohs(ie->length);

        // Check if IE length is valid
        if (offset + 4 + ie_length > length) {
            printf("IE type %d has invalid length %d (exceeds packet boundary)\n",
                   ie_type, ie_length);
            break;
        }

        printf("IE #%d - Type: %d (0x%04X), Length: %d bytes\n",
               ++ie_count, ie_type, ie_type, ie_length);

        // Specifically parse F-TEID (Type 87)
        if (ie_type == 87) { // F-TEID
            f_teid_found++;
            printf("Found F-TEID IE #%d\n", f_teid_found);
            parse_f_teid_ie(ie, ie_length);
        } else {
            // For other IE types, just show basic info
            printf("  Name: %s\n",
                   ie_type == 1 ? "IMSI" :
                   ie_type == 2 ? "Cause" :
                   ie_type == 71 ? "APN" :
                   ie_type == 72 ? "AMBR" :
                   ie_type == 75 ? "IP Address" :
                   ie_type == 77 ? "Indication" :
                   ie_type == 78 ? "PCO" :
                   ie_type == 82 ? "Bearer Context" :
                   ie_type == 83 ? "Charging ID" :
                   ie_type == 86 ? "Bearer Level QoS" :
                   ie_type == 114 ? "UE Time Zone" : "Unknown");
        }

        printf("\n");

        // Move to next IE (4-byte header + IE length, aligned to 4 bytes)
        offset += 4 + ie_length;
        // Align to 4-byte boundary
        if (offset % 4 != 0) {
            offset += 4 - (offset % 4);
        }
    }

    printf("Total IEs parsed: %d\n", ie_count);
    if (!f_teid_found) {
        printf("No F-TEID IEs found in this message\n");
    }
    printf("============================\n");
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
        // Valid IP addresses for our test
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

    // Parse GTPv2 header details
    printf("=== GTPv2 Header Details ===\n");

    // Parse flags
    uint8_t version = (gtp->flags >> 5) & 0x07;  // Bits 5-7: Version
    uint8_t piggyback = (gtp->flags >> 4) & 0x01; // Bit 4: Piggybacking flag
    uint8_t teid_flag = (gtp->flags >> 3) & 0x01; // Bit 3: TEID flag
    uint8_t spare = gtp->flags & 0x07;            // Bits 0-2: Spare

    printf("Flags:\n");
    printf("  Version: %d\n", version);
    printf("  Piggybacking Flag: %d\n", piggyback);
    printf("  TEID Flag: %d\n", teid_flag);
    printf("  Spare: %d\n", spare);
    printf("  Raw Flags: 0x%02X\n", gtp->flags);

    // Message Type
    printf("Message Type: %d (0x%02X)\n", gtp->message_type, gtp->message_type);

    // Message Length
    uint16_t msg_len = ntohs(gtp->message_length);
    printf("Message Length: %d bytes\n", msg_len);

    // TEID (if present)
    uint32_t teid = ntohl(gtp->teid);
    printf("TEID: %u (0x%08X)\n", teid, teid);

    // Sequence Number (24-bit value)
    uint32_t seq_full = ntohl(gtp->sequence_number);
    uint32_t sequence_number = seq_full & 0x00FFFFFF;  // Last 3 bytes
    uint8_t spare2 = (seq_full >> 24) & 0xFF;         // First byte is spare
    printf("Sequence Number: %u (0x%06X)\n", sequence_number, sequence_number);
    printf("Sequence Number Spare: 0x%02X\n", spare2);
    printf("Full Sequence Field: 0x%08X\n", seq_full);
    printf("============================\n");

    // Parse IEs
    int gtp_header_len = sizeof(struct gtpv2_header);
    const uint8_t *ies_data = (const uint8_t *)(packet + sizeof(struct ethhdr) + ip_header_len + udp_header_len + gtp_header_len);
    int ies_length = udp_payload_len - gtp_header_len;

    if (ies_length > 0) {
        parse_gtpv2_ies(ies_data, ies_length, sequence_number);
    } else {
        printf("No IEs found in GTPv2 message\n");
    }

    return 1;
}
