#include "gtpv2_packet_parser.h"

static void print_gtpv2_info(const struct gtpv2_header *gtp, const struct iphdr *ip, const struct udphdr *udp)
{

}

int is_gtpv2_traffic(const unsigned char *packet, int length)
{
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

        printf("  Sequence Number: %u\n", ntohl(gtp->sequence_number));

        // Basic GTPv2 validation
        // Check version (should be 2 for GTPv2)
        if ((gtp->flags & 0xE0) >> 5 != 2) {
            return 0;
        }

        return 1;
}
