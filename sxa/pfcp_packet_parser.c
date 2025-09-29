#include "gtpv2_packet_parser.h"
#include "ue_imsi.h"

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
    uint8_t *value;
} __attribute__((packed));

typedef struct __attribute__((packed)) {
    uint8_t interface_type : 6;
    uint8_t is_ipv6 : 1;
    uint8_t is_ipv4 : 1;
} teid_t;

void parse_fteid_ie(const uint8_t *data, uint16_t length) {
    //    if (length < 2) {
    //        printf("  F-TEID: Invalid length %d (min 2 bytes required)\n", length);
    //        return;
    //    }

    //    struct fteid_ie fteid = {0};
    //    const uint8_t *current = data;

    //    // Parse interface type
    //    fteid.interface_type = current[0];
    //    printf("  F-TEID Interface Type: %u", fteid.interface_type);

    //    // Map interface type to string
    //    const char *interface_names[] = {
    //        "S1-U eNodeB", "S1-U SGW", "S12 RNC", "S12 SGW", "S5/S8-U SGW",
    //        "S5/S8-U PGW", "S5/S8-U SGW (PMIP)", "S5/S8-U PGW (PMIP)", "S11-MME", "S11/S4 SGW"
    //    };
    //    if (fteid.interface_type < sizeof(interface_names)/sizeof(interface_names[0])) {
    //        printf(" (%s)", interface_names[fteid.interface_type]);
    //    }
    //    printf("\n");

    //    // Parse flags
    //    fteid.teid_gre_key_flag = current[1];
    //    printf("  F-TEID Flags: 0x%02X\n", fteid.teid_gre_key_flag);
    //    printf("    TEID/GRE Key present: %u\n", (fteid.teid_gre_key));
}

uint32_t teids[1024 * 4];
uint32_t teids_count = 0;
bool is_teid = false;
void parse_all_ies_recursive(const uint8_t *data, int length, uint32_t message_teid) {

    //    printf("Total IEs length: %d\n", length);

    const uint8_t *current = data;
    const uint8_t *end = data + length;

    is_teid = false;

    while (current < end) {
        // Check if we have enough data for IE header (4 bytes: 1 type + 2 length + 1 spare)
        if (current + 4 > end) {
            printf("Not enough data for IE header. Remaining: %ld bytes\n", (long)(end - current));
            break;
        }

        // Parse IE header - GTPv2 IE format: 1 byte type, 2 bytes length, 1 byte spare
        uint8_t ie_type = current[0];
        uint16_t ie_length = (current[1] << 8) | current[2];
        // current[3] is spare byte (usually 0)

        //           printf("IE Type: 0x%02X, Length: %d", ie_type, ie_length);

        // Check if we have enough data for the IE value
        if (current + 4 + ie_length > end) {
            printf(" - INCOMPLETE (needs %d, has %ld)\n", ie_length, (long)(end - current - 4));
            break;
        }

        // Create IE structure
        struct gtpv2_ie ie;
        ie.type = ie_type;
        ie.length = ie_length;
        ie.value = (uint8_t *)(current + 4);

        if (ie.type == 0x01) {
            char imsi_str[16] = {0};
            parse_imsi_simple(ie.value, ie.length, imsi_str);
            if (strcmp(imsi_str, "999990123456780") == 0) {
//                printf("FOUND MATCH: \n");
                is_teid = true;

            }
        }

        if (ie.type == 0x57) {
            teid_t *teid_ = current + 4;
            uint32_t TEID_GRE_key =  (*(current + 5) << 24) | (*(current + 6) << 16) | (*(current + 7) << 8) | (*(current + 8));

//            printf("    F_TEID_interface_type:  %d -> %d, %d", teid_->interface_type, teid_->is_ipv4, teid_->is_ipv6 );
//            printf("    F_TEID_interface_type:  0x%4X ->(%d) \n", TEID_GRE_key, TEID_GRE_key);

            bool is_consist_array = true;

            if (is_teid) {
                for (int i = 0; i < teids_count; i++) {
                    if (teids[i] == TEID_GRE_key)
                        is_consist_array = false;
                }
                if (is_consist_array)
                teids[teids_count++] = TEID_GRE_key;
            } else {

                for (int i = 0; i < teids_count; i++) {
                    if (teids[i] == message_teid) {
                        for (int i = 0; i < teids_count; i++) {
                            if (teids[i] == TEID_GRE_key)
                                is_consist_array = false;
                        }
                        if (is_consist_array)
                        teids[teids_count++] = TEID_GRE_key;
                    }
                }
            }


        }

        current += 4 + ie_length;
    }

//    printf("\n");


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

    if((strcmp(src_ip, TARGET_CLIENT_IP) == 0) && (strcmp(dst_ip, TARGET_SERVER_IP) == 0) ||
            (strcmp(src_ip, TARGET_SERVER_IP) == 0) && (strcmp(dst_ip, TARGET_CLIENT_IP) == 0)) {
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

    gtp->teid = htonl(gtp->teid);
//    printf("Message Type: %d -> teid_id: %d\n", gtp->message_type, gtp->teid);

    // Message Length
    uint16_t msg_len = ntohs(gtp->message_length);
    //        printf("Message Length: %d bytes\n", msg_len);

    // Calculate available data for IEs
    int gtp_header_size = sizeof(struct gtpv2_header);
    int ies_length = udp_payload_len - gtp_header_size;


    bool cap_this = false;

    if (ies_length > 0) {
        //        printf("len: %d\n", msg_len);
        const unsigned char *ies_data = (const unsigned char *)(packet + sizeof(struct ethhdr) + ip_header_len + udp_header_len + gtp_header_size);

        //        printf("=== Information Elements ===\n");
        parse_all_ies_recursive(ies_data, ies_length, gtp->teid);

        for (int i = 0; i < teids_count; i++) {
//            printf("++++++ TEID:%d:) %d\n", i, teids[i]);
                        if (gtp->teid == teids[i] || is_teid) {
                            cap_this = true;
                        }
        }

        //        is_teid = false;

    }

    //    printf("============================\n");

    return cap_this ? 1 : 0;
}
