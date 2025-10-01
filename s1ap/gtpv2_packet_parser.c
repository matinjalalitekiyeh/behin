#include "gtpv2_packet_parser.h"
#include "ue_imsi.h"

static uint32_t previous_tsn = 0;
static bool first_packet = true;

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.1.1"

static void parse_s1ap_message(const uint8_t *s1ap_data, int s1ap_length) {
    if (s1ap_length < 2) {
        printf("S1AP message too short: %d bytes\n", s1ap_length);
        return;
    }

    printf("\n=== S1AP Message Analysis ===\n");
    printf("Total length: %d bytes\n", s1ap_length);

    /* Print first few bytes for debugging */
    printf("First 8 bytes: ");
    for (int i = 0; i < (s1ap_length < 8 ? s1ap_length : 8); i++) {
        printf("%02x ", s1ap_data[i]);
    }
    printf("\n");

    const uint8_t *ptr = s1ap_data;
    int remaining = s1ap_length;
    int ie_count = 0;

    /* Parse S1AP PDU type and procedure code */
    if (remaining < 1) return;

    uint8_t first_byte = ptr[0];
    int pdu_type = (first_byte >> 6) & 0x03; /* First 2 bits */
    int procedure_code = first_byte & 0x3F;   /* Last 6 bits */

    const char *pdu_type_str = "";
    switch (pdu_type) {
        case 0: pdu_type_str = "InitiatingMessage"; break;
        case 1: pdu_type_str = "SuccessfulOutcome"; break;
        case 2: pdu_type_str = "UnsuccessfulOutcome"; break;
        default: pdu_type_str = "Unknown"; break;
    }

    printf("PDU Type: %s, Procedure Code: %d\n", pdu_type_str, procedure_code);

    ptr += 1;
    remaining -= 1;

    /* Extended procedure code if needed */
    if (procedure_code == 0x3F && remaining >= 1) {
        procedure_code = ptr[0];
        ptr += 1;
        remaining -= 1;
        printf("Extended Procedure Code: %d\n", procedure_code);
    }

    printf("\nProtocol IEs:\n");
    printf("Type\t\tLength\tCriticality\tPresence\n");
    printf("----\t\t------\t----------\t--------\n");

    while (remaining >= 2) {
        uint16_t ie_id = (ptr[0] << 8) | ptr[1];
        ptr += 2;
        remaining -= 2;

        if (remaining < 1) break;

        /* Parse Criticality and Presence */
        uint8_t crit_pres_byte = ptr[0];
        int criticality = (crit_pres_byte >> 6) & 0x03;
        int presence = (crit_pres_byte >> 4) & 0x03;
        ptr += 1;
        remaining -= 1;

        /* Handle extended criticality if present */
        if (criticality == 3) { /* extended */
            if (remaining >= 1) {
                criticality = ptr[0] & 0x03;
                ptr += 1;
                remaining -= 1;
            }
        }

        if (remaining < 1) break;

        /* Parse Length */
        uint8_t len_byte = ptr[0];
        uint32_t ie_length = 0;
        int len_bytes = 1;

        if (len_byte & 0x80) {
            int ext_len_bytes = len_byte & 0x7F;
            if (ext_len_bytes == 1 && remaining >= 2) {
                ie_length = ptr[1];
                len_bytes = 2;
            } else if (ext_len_bytes == 2 && remaining >= 3) {
                ie_length = (ptr[1] << 8) | ptr[2];
                len_bytes = 3;
            } else if (ext_len_bytes == 3 && remaining >= 4) {
                ie_length = (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
                len_bytes = 4;
            } else {
                printf("Invalid length encoding for IE %u\n", ie_id);
                break;
            }
        } else {
            ie_length = len_byte;
        }

        ptr += len_bytes;
        remaining -= len_bytes;

        /* Validate we have enough data */
        if (ie_length > (uint32_t)remaining) {
            printf("IE %u: Length %u exceeds remaining %d\n", ie_id, ie_length, remaining);
            break;
        }

        /* Decode criticality */
        const char *crit_str = "UNKNOWN";
        switch (criticality) {
            case 0: crit_str = "REJECT"; break;
            case 1: crit_str = "IGNORE"; break;
            case 2: crit_str = "NOTIFY"; break;
        }

        /* Decode presence */
        const char *pres_str = "UNKNOWN";
        switch (presence) {
            case 0: pres_str = "OPTIONAL"; break;
            case 1: pres_str = "CONDITIONAL"; break;
            case 2: pres_str = "MANDATORY"; break;
        }

        printf("%-8u\t%-6u\t%-10s\t%s\n", ie_id, ie_length, crit_str, pres_str);
        ie_count++;

        /* Move to next IE */
        ptr += ie_length;
        remaining -= ie_length;
    }

    printf("\nTotal IEs parsed: %d\n", ie_count);
    printf("Remaining unparsed bytes: %d\n", remaining);

    if (remaining > 0) {
        printf("Warning: %d bytes not parsed\n", remaining);
    }

    printf("=== End S1AP Analysis ===\n\n");
}
int is_s1ap_packet(const unsigned char *packet, int length, bool* is_retrans)
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

    bool has_s1ap = false;
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

                if (ppid == SCTP_PPID_S1AP) {
                    has_s1ap = true;

                    /* Check for retransmission */
                    if (!first_packet && tsn == previous_tsn) {
                        *is_retrans = true;
                    }

                    previous_tsn = tsn;
                    first_packet = false;

                    /* Calculate S1AP payload position and length */
                    int s1ap_payload_offset = sizeof(sctp_data_chunk_t);
                    int s1ap_payload_length = chunk_length - s1ap_payload_offset;

                    if (s1ap_payload_length > 0) {
                        const uint8_t *s1ap_data = chunk_ptr + s1ap_payload_offset;

                        if (!(*is_retrans)) {
                            parse_s1ap_message(s1ap_data, s1ap_payload_length);
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

    return has_s1ap ? 1 : 0;
}
