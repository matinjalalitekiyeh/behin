#include "gtpv2_packet_parser.h"

struct gtpv2_header {
    uint8_t flags;
    uint8_t message_type;
    uint16_t message_length;
    uint32_t teid; // tunnel_endpoint_identifier
    uint32_t sequence_number;
}__attribute__((packed));

int is_gtpv2_traffic(const unsigned char *packet, int length, bool* is_retrans)
{
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

        } else {return 0;}


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

           // Parse and print GTPv2 header details
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



        return 1;
}
