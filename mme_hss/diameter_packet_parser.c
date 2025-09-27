#include "diameter_packet_parser.h"

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.0.8"
#define OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION               316
#define OGS_DIAM_S6A_CMD_CODE_CANCEL_LOCATION               317
#define OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION    318
#define OGS_DIAM_S6A_CMD_CODE_INSERT_SUBSCRIBER_DATA        319
#define OGS_DIAM_S6A_CMD_CODE_PURGE_UE                      321


static uint32_t s_hop_id[1000];
static uint32_t s_end_id[1000];
static uint32_t s_user_cnt = 0;

#define IMSI_CAPTURE "999990123456780"

static bool is_cap = false;

typedef struct {
    uint8_t rsv4 : 1;
    uint8_t rsv3 : 1;
    uint8_t rsv2 : 1;
    uint8_t rsv1 : 1;
    uint8_t retransmit : 1;
    uint8_t error : 1;
    uint8_t prox : 1;
    uint8_t req : 1;
} flags_t;

// Structure to represent a parent AVP
typedef struct diam_avp {
    uint32_t avp_code;
    uint32_t vendor_id;
    uint32_t length;
    uint8_t flags;
    uint8_t *data;
    uint32_t data_length;
    struct diam_avp *next;
} diam_avp_t;

static uint32_t read_avp_length(const uint8_t *length_bytes) {
    return (length_bytes[0] << 16) | (length_bytes[1] << 8) | length_bytes[2];
}
static int calculate_padding(uint32_t length) {
    return (4 - (length % 4)) % 4;
}
static diam_avp_t* parse_avps(const uint8_t *data, int data_length) {
    const uint8_t *ptr = data;
    int remaining_length = data_length;
    diam_avp_t *parent_list = NULL;
    diam_avp_t *parent_tail = NULL;

    while (remaining_length >= 8) {
        const uint8_t *avp_start = ptr;

        // Read AVP code
        uint32_t avp_code = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
        ptr += 4;
        remaining_length -= 4;

        // Read flags
        uint8_t flags = *ptr++;
        remaining_length--;

        // Read length (3 bytes)
        uint32_t avp_length = read_avp_length(ptr);
        ptr += 3;
        remaining_length -= 3;

        // Check if we have enough data for this AVP
        if (avp_length > remaining_length + 8) {
            printf("AVP length exceeds remaining data\n");
            break;
        }

        // Read Vendor-ID if V-bit is set
        uint32_t vendor_id = 0;
        uint32_t header_size = 8;

        if (flags & 0x80) {
            if (remaining_length < 4) {
                printf("Not enough data for Vendor-ID\n");
                break;
            }
            vendor_id = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
            ptr += 4;
            remaining_length -= 4;
            header_size = 12;
        }

        // Calculate data length
        uint32_t data_length_avp = avp_length - header_size;
        if (data_length_avp > remaining_length) {
            printf("AVP data length exceeds remaining data\n");
            break;
        }

        // Store the current position as data start
        const uint8_t *data_start = ptr;

        // Check if this is a parent AVP
        int is_parent = (flags & 0x40) != 0;

        if (data_length_avp >= 8) {
            const uint8_t *data_ptr = ptr;
            int data_remaining = data_length_avp;

            while (data_remaining >= 8) {
                uint32_t nested_length = read_avp_length(data_ptr + 5);
                if (nested_length >= 8 && nested_length <= data_remaining) {
                    is_parent = 1;
                    break;
                }
                data_remaining -= nested_length;
                data_ptr += nested_length;
            }
        }

        if (is_parent) {
            diam_avp_t *parent_avp = malloc(sizeof(diam_avp_t));
            if (parent_avp == NULL) {
                printf("Memory allocation failed for parent AVP\n");
                break;
            }

            parent_avp->avp_code = avp_code;
            parent_avp->vendor_id = vendor_id;
            parent_avp->length = avp_length;
            parent_avp->flags = flags;

            // Store AVP data content
            parent_avp->data_length = data_length_avp;
            parent_avp->data = malloc(data_length_avp);
            if (parent_avp->data == NULL) {
                printf("Memory allocation failed for AVP data\n");
                free(parent_avp);
                break;
            }
            memcpy(parent_avp->data, data_start, data_length_avp);

            parent_avp->next = NULL;

            if (parent_list == NULL) {
                parent_list = parent_avp;
                parent_tail = parent_avp;
            } else {
                parent_tail->next = parent_avp;
                parent_tail = parent_avp;
            }
        }

        // Skip AVP data
        ptr += data_length_avp;
        remaining_length -= data_length_avp;

        // Skip padding
        int padding = calculate_padding(avp_length);
        if (padding > 0 && padding <= remaining_length) {
            ptr += padding;
            remaining_length -= padding;
        }
    }

    return parent_list;
}
static void free_avp_list(diam_avp_t *avp_list) {
    while (avp_list != NULL) {
        diam_avp_t *next = avp_list->next;
        if (avp_list->data != NULL) {
            free(avp_list->data);
        }
        free(avp_list);
        avp_list = next;
    }
}
static void print_parent_avps(diam_avp_t *parent_avps, uint32_t end_id, uint32_t hop_id) {

    if (parent_avps == NULL) {
        printf("No parent AVPs found\n");
        return;
    }

    diam_avp_t *current = parent_avps;
    while (current != NULL) {
        if (current->avp_code == 0x00000001) {
            char *content_str = malloc(current->data_length + 1);
            if (content_str != NULL) {
                memcpy(content_str, current->data, current->data_length);
                content_str[current->data_length] = '\0';

                if (strcmp(content_str, "999990123456780") == 0) {
                    is_cap = true;
                    s_end_id[s_user_cnt] = end_id;
                    s_hop_id[s_user_cnt] = hop_id;
                    s_user_cnt++;
                }

                free(content_str);
            }
        }

        current = current->next;
    }
}
static void parse_diam_message(const uint8_t *diam_data, int diam_length) {
    if (diam_length < 20) {
        printf("Diameter message too short (%d bytes)\n", diam_length);
        return;
    }

    uint32_t app_id = (diam_data[8] << 24) | (diam_data[9] << 16) | (diam_data[10] << 8) | diam_data[11];
    uint32_t hop_id = (diam_data[12] << 24) | (diam_data[13] << 16) | (diam_data[14] << 8) | diam_data[15];
    uint32_t end_id = (diam_data[16] << 24) | (diam_data[17] << 16) | (diam_data[18] << 8) | diam_data[19];

    for (uint32_t i = 0; i < s_user_cnt; i++) {
        if (s_hop_id[i] == hop_id && s_end_id[i] == end_id) {
            is_cap = true;
        }
    }

    /* s6a/s6d */
    if (0x1000023 == app_id) {
        // Parse AVPs in the message body
        int avp_data_length = diam_length - 20;
        if (avp_data_length > 0) {
            diam_avp_t *parent_avps = parse_avps(diam_data + 20, avp_data_length);
            print_parent_avps(parent_avps, end_id, hop_id);
            free_avp_list(parent_avps);
        }
    } else { }
}

int is_diam_packet(const unsigned char *packet, int length, bool *is_retrans)
{
    *is_retrans = false;
    is_cap = false;

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

    return has_diam&&is_cap ? 1 : 0;
}
