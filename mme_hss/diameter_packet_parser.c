#include "diameter_packet_parser.h"

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.0.8"

#define OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION               316
#define OGS_DIAM_S6A_CMD_CODE_CANCEL_LOCATION               317
#define OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION    318
#define OGS_DIAM_S6A_CMD_CODE_INSERT_SUBSCRIBER_DATA        319
#define OGS_DIAM_S6A_CMD_CODE_PURGE_UE                      321

typedef struct {
    uint8_t rsv4 : 1;
    uint8_t rsv3 : 1;
    uint8_t rsv2 : 1;
    uint8_t rsv1 : 1;
    uint8_t retransmit : 1;
    uint8_t error : 1;
    uint8_t prox : 1;
    uint8_t req : 1;
}   flags_t ;

static void parse_diam_message(const uint8_t *diam_data, int diam_length) {
    if (diam_length < 20) {
        printf("Diameter message too short (%d bytes)\n", diam_length);
        return;
    }

    // Parse header fields directly from byte array
    uint8_t version = diam_data[0];
    uint32_t length = (diam_data[1] << 16) | (diam_data[2] << 8) | diam_data[3];
    flags_t* flags = &diam_data[4];
    uint32_t cmd_code = (diam_data[5] << 16) | (diam_data[6] << 8) | diam_data[7];
    uint32_t app_id = (diam_data[8] << 24) | (diam_data[9] << 16) | (diam_data[10] << 8) | diam_data[11];
    uint32_t hop_id = (diam_data[12] << 24) | (diam_data[13] << 16) | (diam_data[14] << 8) | diam_data[15];
    uint32_t end_id = (diam_data[16] << 24) | (diam_data[17] << 16) | (diam_data[18] << 8) | diam_data[19];


    /* s6a/s6d */
    if (0x1000023 == app_id) {

        //        printf("req: %d - proxy: %d - error: %d - retrans: %d", f->req ? 1 : 0, f->prox ? 1 : 0, f->error ? 1 : 0, f->retransmit ? 1 : 0);

        switch (cmd_code) {
        case OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION: {
            if (flags->req) {
                printf("REQUEST OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION\n");
            } else {
                printf("RESPONSE OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION\n");
            }
        } break;
        case OGS_DIAM_S6A_CMD_CODE_CANCEL_LOCATION: {
            if (flags->req) {
                printf("REQUEST OGS_DIAM_S6A_CMD_CODE_CANCEL_LOCATION\n");
            } else {
                printf("RESPONSE OGS_DIAM_S6A_CMD_CODE_CANCEL_LOCATION\n");
            }
        } break;
        case OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION: {
            if (flags->req) {
                printf("REQUEST OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION\n");
            }else {
                printf("RESPONSE OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION\n");
            }
        } break;
        case OGS_DIAM_S6A_CMD_CODE_INSERT_SUBSCRIBER_DATA: {
            if (flags->req) {
                printf("REQUEST OGS_DIAM_S6A_CMD_CODE_INSERT_SUBSCRIBER_DATA\n");
            }else {
                printf("RESPONSE OGS_DIAM_S6A_CMD_CODE_INSERT_SUBSCRIBER_DATA\n");
            }
        } break;
        case OGS_DIAM_S6A_CMD_CODE_PURGE_UE: {
            if (flags->req) {
                printf("REQUEST OGS_DIAM_S6A_CMD_CODE_PURGE_UE\n");
            }else {
                printf("RESPONSE OGS_DIAM_S6A_CMD_CODE_PURGE_UE\n");
            }
        } break;
        default: break;
        }
    }


    //    bool is_request =  (flags & 0x80);16777251
    //    /* s6a/s6d */
    //    if (app_id == 16777251 && is_request) {

    //        printf("%u\n", cmd_code);
    //            int idx = 0;
    //            for (int i = 0; i < diam_length - 4; i++) {
    //                if (diam_data[i] == 0x00 && diam_data[i+1] == 0x00 && diam_data[2+i] == 0x00 && diam_data[3+i] == 0x01) {
    //                    idx = i;
    //                    break;
    //                }
    //            }
    //            long long imsi = imsi_direct_to_long_long(&diam_data[idx + 7]);
    //            printf("FUCK KKKKKKKKKKKKKKKKKK  %llu\n", imsi);


    //    }



    //    if (316 == cmd_code) {
    //            int idx = 0;
    //            for (int i = 0; i < diam_length - 4; i++) {
    //                if (diam_data[i] == 0x00 && diam_data[i+1] == 0x00 && diam_data[2+i] == 0x00 && diam_data[3+i] == 0x01) {
    //                    idx = i;
    //                    break;
    //                }
    //            }
    //    }

    // Print basic header info
    //    printf("Diameter: v%d, %s, Cmd:%u, App:%u\n",
    //           version,
    //           (flags & 0x80) ? "REQ" : "ANS",
    //           cmd_code,
    //           app_id);

    // Print IDs
    //    printf("Hop-by-Hop: 0x%08X, End-to-End: 0x%08X\n", hop_id, end_id);

    // Print message length
    //    printf("Length: %u bytes (header + AVPs)\n", length);

    // Check if there are AVPs to parse
    //    if (diam_length > 20) {
    //        printf("AVP data: %d bytes available\n", diam_length - 20);
    //    }

    //    int idx = 0;
    //    for (int i = 0; i < diam_length - 4; i++) {
    //        if (diam_data[i] == 0x00 && diam_data[i+1] == 0x00 && diam_data[2+i] == 0x00 && diam_data[3+i] == 0x01) {
    //            idx = i;
    //            break;
    //        }
    //    }


    //    printf("FUCK KKKKKKKKKKKKKKKKKK  %llu\n", imsi);



}

int is_diam_packet(const unsigned char *packet, int length, bool *is_retrans)
{
    *is_retrans = false;

    // Simple array to track recent messages (circular buffer)
#define MAX_RECENT_MSGS 100
    static uint32_t recent_hop_ids[MAX_RECENT_MSGS] = {0};
    static uint32_t recent_end_ids[MAX_RECENT_MSGS] = {0};
    static int current_index = 0;
    static int initialized = 0;

    /* Initialize the array if first run */
    if (!initialized) {
        memset(recent_hop_ids, 0, sizeof(recent_hop_ids));
        memset(recent_end_ids, 0, sizeof(recent_end_ids));
        initialized = 1;
    }

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
    if (ip->protocol != SCTP_PROTOCOL_NO) {
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

    // Check if this is our target traffic
    int is_target_traffic = 0;
    if ((strcmp(src_ip, TARGET_CLIENT_IP) == 0 && strcmp(dst_ip, TARGET_SERVER_IP) == 0) ||
            (strcmp(src_ip, TARGET_SERVER_IP) == 0 && strcmp(dst_ip, TARGET_CLIENT_IP) == 0)) {
        is_target_traffic = 1;
    }

    if (!is_target_traffic) {
        return 0;
    }

    /* Now parse SCTP chunks */
    const unsigned char *sctp_start = packet + sizeof(struct ethhdr) + ip_header_len;
    unsigned long long sctp_length = length - (sizeof(struct ethhdr) + ip_header_len);

    if (sctp_length < sizeof(sctp_common_header_t)) {
        return 0;
    }

    const unsigned char *chunk_ptr = sctp_start + sizeof(sctp_common_header_t);
    unsigned long long remaining_length = sctp_length - sizeof(sctp_common_header_t);

    bool has_diam = false;
    uint32_t current_hop_id = 0;
    uint32_t current_end_id = 0;

    while (remaining_length >= sizeof(sctp_chunk_header_t)) {
        const sctp_chunk_header_t *chunk = (const sctp_chunk_header_t*)chunk_ptr;
        uint16_t chunk_length = ntohs(chunk->length);

        if (chunk_length == 0 || chunk_length > remaining_length) {
            break;
        }

        if (chunk->type == 0) { // DATA chunk
            if (chunk_length >= sizeof(sctp_data_chunk_t)) {
                const sctp_data_chunk_t *data_chunk = (const sctp_data_chunk_t*)chunk_ptr;
                uint32_t ppid = ntohl(data_chunk->protocol_id);

                if (ppid == SCTP_PPID_DIAM) {
                    has_diam = true;

                    int diam_payload_offset = sizeof(sctp_data_chunk_t);
                    int diam_payload_length = chunk_length - diam_payload_offset;

                    if (diam_payload_length >= 20) { // Minimum Diameter header
                        const uint8_t *diameter_data = chunk_ptr + diam_payload_offset;

                        // Extract Diameter identifiers
                        current_hop_id = ntohl(*(uint32_t*)(diameter_data + 4));
                        current_end_id = ntohl(*(uint32_t*)(diameter_data + 8));

                        // Check if this is a retransmission
                        for (int i = 0; i < MAX_RECENT_MSGS; i++) {
                            if (recent_hop_ids[i] == current_hop_id &&
                                    recent_end_ids[i] == current_end_id) {
                                *is_retrans = true;
                                break;
                            }
                        }

                        // Store this message in recent messages (if not retransmission)
                        if (!(*is_retrans)) {
                            recent_hop_ids[current_index] = current_hop_id;
                            recent_end_ids[current_index] = current_end_id;
                            current_index = (current_index + 1) % MAX_RECENT_MSGS;

                            parse_diam_message(diameter_data, diam_payload_length);
                        }
                    }
                }
            }
        }

        // Move to next chunk
        int padded_length = (chunk_length + 3) & ~3;
        chunk_ptr += padded_length;
        remaining_length -= padded_length;

        if (remaining_length <= 0) {
            break;
        }
    }

    return has_diam ? 1 : 0;
}
