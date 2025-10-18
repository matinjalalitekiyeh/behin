#include "s1ap_packet_parser.h"
#include "ue_imsi.h"

static uint32_t previous_tsn = 0;
static bool first_packet = true;

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.1.1"


uint32_t enb_ue_ids[1024 * 4];
uint32_t mme_ue_ids[1024 * 4];
uint32_t enb_ue_id_count = 0;
uint32_t mme_ue_id_count = 0;


uint32_t counter_mme  = 0;
uint32_t counter_enb  = 0;

bool __init = false;

__attribute__((constructor))
void initialize(void) {
    enb_ue_ids[enb_ue_id_count++] = 0;
    mme_ue_ids[mme_ue_id_count++] = 99;
}

static uint32_t get_enb_ue_id(const uint8_t* message, int len)
{
    if (len < 4) return 0;

    uint32_t enb_ue_s1ap_id = 0x00;

    int indexes[200];
    int idx = 0;
    for (int i = 0; i < len - 4; i++) {
        if (message[i] == 0x00 && message[i+1] == 0x08) {
            idx = i;
            indexes[counter_enb] = i;
            counter_enb++;
	    if (counter_enb > 198) {
		    break;
	    }
        }
    }

    for (int j = 0; j < counter_enb; j++) {
        idx = indexes[j];

        uint16_t protocol_field = htons( *(uint16_t*)&message[idx] );

        /*id-eNB-UE-S1AP-ID*/
        if (protocol_field == 0x0008) {

            uint8_t check_criticality = ( *(uint8_t*)&message[(idx + 2)] );

            int len_idx = 0;
            if ((check_criticality == 0x40) || (check_criticality == 0x00) || (check_criticality == 0x80)) {
                len_idx = idx + 3;
            } else {
                len_idx = idx + 2;
            }

            uint8_t enb_ue_s1ap_id_len = ( *(uint8_t*)&message[len_idx] );

            if ((enb_ue_s1ap_id_len > 4) || (enb_ue_s1ap_id_len == 0)) {
                continue;
            }

            idx = len_idx + 1;
            if (enb_ue_s1ap_id_len == 1) {
                enb_ue_s1ap_id = message[idx];
            } else if (enb_ue_s1ap_id_len == 2) {
                enb_ue_s1ap_id = (message[idx] << 8) | (message[idx+1] << 0);
            } else if (enb_ue_s1ap_id_len == 3) {
                enb_ue_s1ap_id = (message[idx] << 16) | (message[idx+1] << 8) | (message[idx+2] << 0);
            } else if (enb_ue_s1ap_id_len == 4) {
                enb_ue_s1ap_id = (message[idx] << 24) | (message[idx+1] << 16) | (message[idx+2] << 8) | (message[idx+3] << 0);
            }
        }
    }

    return enb_ue_s1ap_id;
}

static uint32_t get_mme_ue_id(const uint8_t* message, int len)
{
    if (len < 4) return 0;

    uint32_t mme_ue_s1ap_id = 0x00;

    int indexes[200];
    int idx = 0;
    for (int i = 0; i < len - 4; i++) {
        if (message[i] == 0x00 && message[i+1] == 0x00) {
            idx = i;
            indexes[counter_mme] = i;
            counter_mme++;
	    if (counter_mme > 198) {
		    break;
	    }
        }
    }

    for (int j = 0; j < counter_mme; j++) {
        idx = indexes[j];

        uint16_t protocol_field = htons( *(uint16_t*)&message[idx] );

        /*id-MME-UE-S1AP-ID*/
        if (protocol_field == 0x0000) {

            uint8_t check_criticality = ( *(uint8_t*)&message[(idx + 2)] );

            int len_idx = 0;
            if ((check_criticality == 0x40) || (check_criticality == 0x00) || (check_criticality == 0x80)) {
                len_idx = idx + 3;
            } else {
                len_idx = idx + 2;
            }

            uint8_t mme_ue_s1ap_id_len = ( *(uint8_t*)&message[len_idx] );

            if ((mme_ue_s1ap_id_len > 4) || (mme_ue_s1ap_id_len == 0)) {
                continue;
            }

            idx = len_idx + 1;
            if (mme_ue_s1ap_id_len == 1) {
                mme_ue_s1ap_id = message[idx];
            } else if (mme_ue_s1ap_id_len == 2) {
                mme_ue_s1ap_id = (message[idx] << 8) | (message[idx+1] << 0);
            } else if (mme_ue_s1ap_id_len == 3) {
                mme_ue_s1ap_id = (message[idx] << 16) | (message[idx+1] << 8) | (message[idx+2] << 0);
            } else if (mme_ue_s1ap_id_len == 4) {
                mme_ue_s1ap_id = (message[idx] << 24) | (message[idx+1] << 16) | (message[idx+2] << 8) | (message[idx+3] << 0);
            }
        }
    }

    return mme_ue_s1ap_id;
}

static void nas_pdu(const uint8_t *message, int len, uint32_t enb_ue_s1ap_id) {
    int idx = 0;
    for (int i = 0; i < len - 3; i++) {
        if (message[i] == 0x00 && message[i+1] == 0x1a) {
            idx = i;
            break;
        }
    }

    typedef struct  {
        uint8_t protocol_discr:4;
        uint8_t security_header:4;
    }non_access_stream_t;

    non_access_stream_t d;
    memset(&d, 0, sizeof (non_access_stream_t));
    memcpy(&d, &message[idx + 5], sizeof(non_access_stream_t));
    if (d.security_header == 1) {
        idx += 6;
    }

    //    uint8_t nas_eps_mobility_management_message_type = *(uint8_t*)&message[idx + 6];
    //    memcpy(&nas_eps_mobility_management_message_type, &message[idx + 6], sizeof(uint8_t));
    //    if (nas_eps_mobility_management_message_type == 0x41) {
    //            std::cout << "nas_eps_mobility_management_message_type 0x41" << nas_eps_mobility_management_message_type << std::endl;
    //    }else if (nas_eps_mobility_management_message_type == 0x48) {
    //            std::cout << "nas_eps_mobility_management_message_type 0x48" << nas_eps_mobility_management_message_type << std::endl;
    //    }

    /* ignore 7 byte (not important messsage for trace) */

    uint8_t guti_type = *(uint8_t*)&message[idx + 9];
    uint8_t type = (guti_type  & 0x07);

    if (type == 6) {
        uint32_t tmsi = 0x00;
        memcpy(&tmsi, &message[idx + 16], sizeof(uint32_t));
        tmsi = htonl(tmsi);
        printf("tmsi: %d\n", tmsi);
        if (enb_ue_s1ap_id != 0x00) {
            enb_ue_ids[enb_ue_id_count] = enb_ue_s1ap_id;
            __init = true;
        }

    } else if (type == 1) {
        long long imsi = imsi_direct_to_long_long(&message[idx + 9]);
        printf("imsi: %llu\n", imsi);

        if (enb_ue_s1ap_id != 0x00) {
            if (imsi == 999700000000005) {
                printf("FOUND IMSI: %llu\n", imsi);
                enb_ue_ids[enb_ue_id_count] = enb_ue_s1ap_id;
                __init = true;
            }
        }
    }
}

typedef struct  {
    uint32_t enb_ue_s1ap_id;
    uint32_t mme_ue_s1ap_id;
} ue_id_pair_t;

static
ue_id_pair_t parse_s1ap_message(const uint8_t *s1ap_data, int s1ap_length)
{
    ue_id_pair_t pair = {.enb_ue_s1ap_id = 0x00, .mme_ue_s1ap_id = 0x00};

    /*check for init-ue-message*/
    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0c) {
        pair.enb_ue_s1ap_id = get_enb_ue_id(s1ap_data, s1ap_length);

        pair.mme_ue_s1ap_id = get_mme_ue_id(s1ap_data, s1ap_length);

        nas_pdu(s1ap_data, s1ap_length, pair.enb_ue_s1ap_id);
    } else {
        pair.enb_ue_s1ap_id = get_enb_ue_id(s1ap_data, s1ap_length);

        pair.mme_ue_s1ap_id = get_mme_ue_id(s1ap_data, s1ap_length);
    }

    counter_mme = 0; counter_enb = 0;

    if (__init && (pair.mme_ue_s1ap_id != 0x00)) {
        mme_ue_ids[mme_ue_id_count++] = pair.mme_ue_s1ap_id;
        enb_ue_ids[enb_ue_id_count++] = pair.enb_ue_s1ap_id;
        __init = false;
    }

    return pair;
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

    ue_id_pair_t pair = {0};

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
                            pair = parse_s1ap_message(s1ap_data, s1ap_payload_length);
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

    bool cap_this = false;

    for (int i = 0; i < mme_ue_id_count; i++) {
        if (pair.mme_ue_s1ap_id == mme_ue_ids[i]) {
            cap_this = true;
        }
    }
    for (int i = 0; i < enb_ue_id_count; i++) {
        if (pair.enb_ue_s1ap_id == enb_ue_ids[i]) {
            cap_this = true;
        }
    }

    /*For PDN connectivity*/
    if (__init) {
        cap_this = true;
    }


    return has_s1ap&&cap_this ? 1 : 0;
}
