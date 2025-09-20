#include "diameter_packet_parser.h"

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.0.8"

static void parse_diam_message(const uint8_t *diam_data, int diam_length) {
    if (diam_length < 20) {
        printf("Diameter message too short (%d bytes)\n", diam_length);
        return;
    }

    // Parse header fields directly from byte array
    uint8_t version = diam_data[0];
    uint32_t length = (diam_data[1] << 16) | (diam_data[2] << 8) | diam_data[3];
    uint8_t flags = diam_data[4];
    uint32_t cmd_code = (diam_data[5] << 16) | (diam_data[6] << 8) | diam_data[7];
    uint32_t app_id = (diam_data[8] << 24) | (diam_data[9] << 16) | (diam_data[10] << 8) | diam_data[11];
    uint32_t hop_id = (diam_data[12] << 24) | (diam_data[13] << 16) | (diam_data[14] << 8) | diam_data[15];
    uint32_t end_id = (diam_data[16] << 24) | (diam_data[17] << 16) | (diam_data[18] << 8) | diam_data[19];

    // Print basic header info
    printf("Diameter: v%d, %s, Cmd:%u, App:%u\n",
           version,
           (flags & 0x80) ? "REQ" : "ANS",
           cmd_code,
           app_id);

    // Print IDs
    printf("Hop-by-Hop: 0x%08X, End-to-End: 0x%08X\n", hop_id, end_id);

    // Print message length
    printf("Length: %u bytes (header + AVPs)\n", length);

    // Check if there are AVPs to parse
    if (diam_length > 20) {
        printf("AVP data: %d bytes available\n", diam_length - 20);
    }

}

int is_diam_packet(const unsigned char *packet, int length, bool *is_retrans)
{
    *is_retrans = false;

    /* Check minimum size for Ethernet + IP + SCTP headers */
    if (length < (int)(sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(sctp_common_header_t))) {
        return 0;
    }

    /* Check Ethernet type (IP) */
    const struct ethhdr *eth = (const struct ethhdr *)packet;
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        return 0;
    }

    /* Check IP protocol (SCTP - protocol number 132) */
    const struct iphdr *ip = (const struct iphdr *)(packet + sizeof(struct ethhdr));
    if (ip->protocol != SCTP_PROTOCOL_NO) {  // SCTP protocol number
        return 0;
    }

    /* Check IP header length */
    int ip_header_len = ip->ihl * 4;
    if (length < (int)(sizeof(struct ethhdr) + ip_header_len + sizeof(sctp_common_header_t))) {
        return 0;
    }

    /* Convert IPs to strings for verification */
    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->daddr), dst_ip, INET_ADDRSTRLEN);

    // Check if this is our target S1AP traffic
    int is_target_traffic = 0;
    if ((strcmp(src_ip, TARGET_CLIENT_IP) == 0 && strcmp(dst_ip, TARGET_SERVER_IP) == 0) ||
            (strcmp(src_ip, TARGET_SERVER_IP) == 0 && strcmp(dst_ip, TARGET_CLIENT_IP) == 0)) {
        is_target_traffic = 1;
    }

    if (!is_target_traffic) {
        return 0;
    }

    /* Now parse SCTP chunks to find S1AP data */
    const unsigned char *sctp_start = packet + sizeof(struct ethhdr) + ip_header_len;
    unsigned long long sctp_length = length - (sizeof(struct ethhdr) + ip_header_len);

    if (sctp_length < (int)sizeof(sctp_common_header_t)) {
        return 0;
    }

    /* Get SCTP common header */
    //    const sctp_common_header_t *sctp_common = (const sctp_common_header_t*)sctp_start;
    //    uint16_t src_port = ntohs(sctp_common->source_port);
    //    uint16_t dst_port = ntohs(sctp_common->dest_port);


    /* Pointer to first chunk after common header */
    const unsigned char *chunk_ptr = sctp_start + sizeof(sctp_common_header_t);
    unsigned long long remaining_length = sctp_length - sizeof(sctp_common_header_t);

    bool has_diam = false;
    //    bool is_client_to_server = (strcmp(src_ip, TARGET_CLIENT_IP) == 0);
    //    const char* direction = is_client_to_server ? "Client->Server" : "Server->Client";

    while (remaining_length >= (int)sizeof(sctp_chunk_header_t)) {
        const sctp_chunk_header_t *chunk = (const sctp_chunk_header_t*)chunk_ptr;
        uint16_t chunk_length = ntohs(chunk->length);

        if (chunk_length == 0 || chunk_length > (uint16_t)remaining_length) {
            break;  /* Invalid chunk length */
        }

        if (chunk->type == 0) {
            if (chunk_length >= sizeof(sctp_data_chunk_t)) {
                const sctp_data_chunk_t *data_chunk = (const sctp_data_chunk_t*)chunk_ptr;
                uint32_t ppid = ntohl(data_chunk->protocol_id);
                uint32_t tsn = ntohl(data_chunk->tsn);

                if (ppid == SCTP_PPID_DIAM) {
                    has_diam = true;

                    /* Calculate S1AP payload position and length */
                    int diam_payload_offset = sizeof(sctp_data_chunk_t);
                    int diam_payload_length = chunk_length - diam_payload_offset;

                    if (diam_payload_length > 0) {
                        const uint8_t *diameter_data = chunk_ptr + diam_payload_offset;

                        if (!(*is_retrans)) {
                            parse_diam_message(diameter_data, diam_payload_length);
                        }
                    }
                }
            }
        }

        /* Move to next chunk (chunk length is padded to 4 bytes) */
        int padded_length = (chunk_length + 3) & ~3;
        chunk_ptr += padded_length;
        remaining_length -= padded_length;

        if (remaining_length <= 0) {
            break;
        }
    }

    return has_diam ? 1 : 0;
}
