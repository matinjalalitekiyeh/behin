#include "pfcp_packet_parser.h"
#include "ue_imsi.h"

// PFCP Header structures according to 3GPP TS 29.244
typedef struct __attribute__((packed)) pfcp_header_base_s {
    uint8_t flags;
    uint8_t message_type;
    uint16_t message_length;
} pfcp_header_base_t;

typedef struct __attribute__((packed)) pfcp_header_with_seid_s {
    pfcp_header_base_t base;
    uint64_t seid;
    uint32_t sequence_number;
    uint8_t message_priority;
} pfcp_header_with_seid_t;

typedef struct __attribute__((packed)) pfcp_header_without_seid_s {
    pfcp_header_base_t base;
    uint32_t sequence_number;
    uint8_t message_priority;
} pfcp_header_without_seid_t;

// PFCP Message Types
#define PFCP_HEARTBEAT_REQUEST 1
#define PFCP_HEARTBEAT_RESPONSE 2

void parse_all_ies_recursive(const uint8_t *data, int length) {
//    printf("Total IEs length: %d\n", length);

    data++;
    const uint8_t *current = data;
    const uint8_t *end = data + length;

    while (current < end) {
        // Check if we have enough data for IE header (4 bytes: 2 type + 2 length)
        if (current + 4 > end) {
            printf("Not enough data for IE header. Remaining: %ld bytes\n", (long)(end - current));
            break;
        }

        // Parse IE header - PFCP IE format: 2 bytes type, 2 bytes length (big-endian)
        uint16_t ie_type = (current[0] << 8) | current[1];
        uint16_t ie_length = (current[2] << 8) | current[3];
        ie_length = htons(ie_length);
        ie_length = htons(ie_length);

//        printf("IE Type: ");
//        for (int i = 0 ; i < 50; i++) {
//            printf("0x%02X ", current[i]);
//        }
//        printf("\n\n");
        printf("type: 0x%04X (%d) -- Len: %d\n", ie_type, ie_type, ie_length);

        // Check if we have enough data for the IE value
        if (current + 4 + ie_length > end) {
            printf("INCOMPLETE IE - needs %d, has %ld\n", ie_length, (long)(end - current - 4));
            break;
        }

        // Move to next IE
        current += 4 + ie_length;
    }
}

int is_gtpv2_traffic(const unsigned char *packet, int length, bool* is_retrans) {
    *is_retrans = false;

    // Minimum size check for Ethernet + IP + UDP + GTPv2 header
    if (length < (int)(sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(pfcp_header_base_t))) {
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

    if((strcmp(src_ip, TARGET_CLIENT_IP) == 0) && (strcmp(dst_ip, TARGET_SERVER_IP) == 0) ||
            (strcmp(src_ip, TARGET_SERVER_IP) == 0) && (strcmp(dst_ip, TARGET_CLIENT_IP) == 0)) {
    } else {
        return 0;
    }

    if (ntohs(udp->dest) != PFCP_PORT && ntohs(udp->source) != PFCP_PORT) {
        return 0;
    }

    // Calculate UDP payload length
    int udp_header_len = sizeof(struct udphdr);
    int udp_payload_len = ntohs(udp->len) - udp_header_len;

    // Check if we have enough data for GTPv2 header
    if (udp_payload_len < (int)sizeof(pfcp_header_base_t)) {
        return 0;
    }

    // Parse PFCP base header
       pfcp_header_base_t *base_header = (pfcp_header_base_t *)(packet + sizeof(struct ethhdr) + ip_header_len + udp_header_len);

       // Check if this is a heartbeat message and ignore it
       if (base_header->message_type == PFCP_HEARTBEAT_REQUEST ||
           base_header->message_type == PFCP_HEARTBEAT_RESPONSE) {
//           printf("Ignoring heartbeat message (Type: %d)\n", base_header->message_type);
           return 0;
       }

       // Extract flags
       uint8_t version = (base_header->flags >> 5) & 0x07;
       uint8_t mp_flag = (base_header->flags >> 1) & 0x01;
       uint8_t seid_present = base_header->flags & 0x01;

       // Calculate header size
       int pfcp_header_size = sizeof(pfcp_header_base_t);
       if (seid_present) {
           pfcp_header_size += 11; // SEID (8) + Sequence (3) + Spare (1)
       } else {
           pfcp_header_size += 4;  // Sequence (3) + Spare (1)
       }
       if (mp_flag) {
           pfcp_header_size += 1;  // Message Priority
       }

       // Calculate IEs data length
       int pfcp_ies_length = udp_payload_len - pfcp_header_size;

//       printf("PFCP Message - Type: 0x%02X, Length: %u, Version: %d, MP: %d, SEID: %d\n",
//              base_header->message_type, ntohs(base_header->message_length),
//              version, mp_flag, seid_present);

       if (pfcp_ies_length > 0) {
           const unsigned char *ies_data = (const unsigned char *)(packet + sizeof(struct ethhdr) + ip_header_len + udp_header_len + pfcp_header_size);

           // Parse SEID and Sequence Number if present
           if (seid_present) {
               pfcp_header_with_seid_t *header = (pfcp_header_with_seid_t *)base_header;
               uint64_t seid = be64toh(header->seid);
               uint32_t seq_num = ntohl(header->sequence_number) >> 8;
//               ies_data += sizeof(pfcp_header_with_seid_t);
//               printf("SEID: 0x%016lX, Sequence: %u\n", seid, seq_num);
           } else {
               pfcp_header_without_seid_t *header = (pfcp_header_without_seid_t *)base_header;
               uint32_t seq_num = ntohl(header->sequence_number) >> 8;
//               ies_data += sizeof(pfcp_header_without_seid_t);
//               printf("Sequence: %u\n", seq_num);
           }

           printf("\n=== ====================== ===\n");
           parse_all_ies_recursive(ies_data, pfcp_ies_length);
//           printf("=== Finished IE Parsing ===\n\n");
       }
    return 1;
}
