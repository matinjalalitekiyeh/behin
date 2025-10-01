#include "gtpv2_packet_parser.h"
#include "ue_imsi.h"

static uint32_t previous_tsn = 0;
static bool first_packet = true;

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.1.1"

typedef struct  {
    uint8_t msb:4;
    uint8_t lsb:4;
}data_t;


typedef struct  {
    uint8_t attach_type : 3;
    uint8_t spare_bit : 1;
    uint8_t nas_key_set_identifier : 3;
    uint8_t tsc : 1;
}eps_type_t;


void parse_s1ap_message(const uint8_t *s1ap_data, int s1ap_length)
{
    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0c && s1ap_data[2] == 0x40) {



        int idx = 0;
        for (int i = 0; i < s1ap_length - 2; i++) {
            if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x08) {
                idx = i;
                break;
            }
        }

        uint16_t protocol_field = htons( *(uint16_t*)&s1ap_data[idx] );
        //        std::cout << "protocol_field: " << protocol_field << std::endl;

        if (protocol_field == 0x0008) {
            const int len_idx = idx + (int)(sizeof (uint16_t));
            uint16_t enb_ue_s1ap_id_len = htons( *(uint16_t*)&s1ap_data[len_idx] );

            const int enb_idx = len_idx + (int)(sizeof (uint16_t));
            uint32_t enb_ue_s1ap_id = 0x00;
            memcpy(&enb_ue_s1ap_id, &s1ap_data[enb_idx], enb_ue_s1ap_id_len);


            if (enb_ue_s1ap_id_len == 2) {
                enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
            } else if (enb_ue_s1ap_id_len == 4) {
                enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
            }

            printf("\n *********** ENB UE S1AP ID: %d\n", enb_ue_s1ap_id);

//            std::cout << "\n *********** ENB UE S1AP ID: " << enb_ue_s1ap_id  << '\n' << std::endl;

        }



        int idx2 = 0;
        for (int i = 0; i < s1ap_length - 3; i++) {
            if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x1a && s1ap_data[i+2] == 0x00) {
                idx2 = i;
                break;
            }
        }

        data_t d;
        memset(&d, 0, sizeof (data_t));
        memcpy(&d, &s1ap_data[idx2 + 5], sizeof(data_t));
        if (d.lsb == 0) {
            //            std::cout << "ok " << std::bitset<4>(d.msb) << ' ' << std::bitset<4>(d.lsb) << std::endl;
        } else if (d.lsb == 1) {
            //            std::cout << "should shift 6 byte " << std::bitset<4>(d.lsb) << std::endl;
            idx2 += 6;
        }


        uint8_t nas_eps_mobility_management_message_type = *(uint8_t*)&s1ap_data[idx2 + 6];
        memcpy(&nas_eps_mobility_management_message_type, &s1ap_data[idx2 + 6], sizeof(uint8_t));
        if (nas_eps_mobility_management_message_type == 0x41) {
//            std::cout << "nas_eps_mobility_management_message_type 0x41" << nas_eps_mobility_management_message_type << std::endl;
        }else if (nas_eps_mobility_management_message_type == 0x48) {
//            std::cout << "nas_eps_mobility_management_message_type 0x48" << nas_eps_mobility_management_message_type << std::endl;
        }

        //7

        uint8_t guti_type = *(uint8_t*)&s1ap_data[idx2 + 9];
        uint8_t type = (guti_type  & 0x07);

        if (type == 6) {
            uint32_t tmsi = 0x00;
            memcpy(&tmsi, &s1ap_data[idx2 + 16], sizeof(uint32_t));
            tmsi = htonl(tmsi);
            printf("tmsi: %d\n", tmsi);
        } else if (type == 1) {
//            char imsi_str[16] = {0};
            long long imsi = imsi_direct_to_long_long(&s1ap_data[idx2 + 9]);
//            parse_imsi_simple(&s1ap_data[idx2 + 9], 16, imsi_str);
            printf("imsi: %llu\n", imsi);
        }




    } else {

                int indx = 0;
                for (int i = 0; i < s1ap_length - 4; i++) {
                    if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x08) {
                        indx = i;
                        break;
                    }
                }
                uint16_t protocol_field = htons( *(uint16_t*)&s1ap_data[indx] );

                if (protocol_field == 0x0008) {
                    const int len_idx = indx + (int)(sizeof (uint16_t));//todo
                    uint16_t enb_ue_s1ap_id_len = htons( *(uint16_t*)&s1ap_data[len_idx] );

                    if (enb_ue_s1ap_id_len > 4)
                        return;

                    const int enb_idx = len_idx + (int)(sizeof (uint16_t));
                    uint32_t enb_ue_s1ap_id = 0x00;
                    memcpy(&enb_ue_s1ap_id, &s1ap_data[enb_idx], enb_ue_s1ap_id_len);
                                        if (enb_ue_s1ap_id_len == 2) {
                                            enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
                                        } else if (enb_ue_s1ap_id_len == 4) {
                                            enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
                                        }

                                        printf("\n *********** ENB UE S1AP ID: %d\n", enb_ue_s1ap_id);

                            //            std::cout << "\n *********** ENB UE S1AP ID: " << enb_ue_s1ap_id  << '\n' << std::endl;
                }

                for (int i = 0; i < s1ap_length - 4; i++) {
                    if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x00 && s1ap_data[i+2] == 0x00) {
                        indx = i;
                        break;
                    }
                }
                uint16_t protocol_field2 = htons( *(uint16_t*)&s1ap_data[indx] );
//                printf("+++++++++++++++++++++++++++++++++++++++fuck: %d -- protocole field %d\n", indx, protocol_field2);

                if (protocol_field2 == 0x0000) {
                    indx += 3;
                    uint8_t len = s1ap_data[indx++];
                    indx++;
                    uint16_t mme_ue = (s1ap_data[indx] << 8) | s1ap_data[indx+1];
                    for(int i = 0; i < len; i++) {
                        mme_ue = (mme_ue << 8) | s1ap_data[indx + i];
                    }
//                    memcpy(&mme_ue, &s1ap_data[indx], len);
                    printf("MME UE (%d): 0x%4X\n", len, mme_ue);
//                    if (len == 2) {
//                        htons
//                    } else if (len == 3) {

//                    } else {

//                    }

//                    const int len_idx = indx + (int)(sizeof (uint16_t));
//                    uint16_t enb_ue_s1ap_id_len = htons( *(uint16_t*)&s1ap_data[len_idx] );

//                    if (enb_ue_s1ap_id_len > 4)
//                        return;

//                    const int enb_idx = len_idx + (int)(sizeof (uint16_t));
//                    uint32_t enb_ue_s1ap_id = 0x00;
//                    memcpy(&enb_ue_s1ap_id, &s1ap_data[enb_idx], enb_ue_s1ap_id_len);
//                                        if (enb_ue_s1ap_id_len == 2) {
//                                            enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
//                                        } else if (enb_ue_s1ap_id_len == 4) {
//                                            enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
//                                        }

//                                        printf("\n ++++++++++++++++++++++++++++++  MME UE S1AP ID: %d\n", enb_ue_s1ap_id);

//                            //            std::cout << "\n *********** ENB UE S1AP ID: " << enb_ue_s1ap_id  << '\n' << std::endl;
                }


//                }

//        uint8_t len = *(uint8_t*)&s1ap_data[10];
//        uint32_t enb = 0x00;
//        memcpy(&enb, &s1ap_data[11], len);
//        if (len == 2) {
//            enb = htons(enb);
//        } else if (len == 4) {
//            enb = htonl(enb);
//        }
//        uint16_t xxx =  *(uint16_t*)&s1ap_data[11] ;
//        xxx = htons(xxx);

//        uint16_t xxx2 =  *(uint16_t*)&s1ap_data[17] ;
//        xxx2 = htons(xxx2);

//        printf("ENB: %d ---- UE: %d\n", xxx ,xxx2);




//        int indx = 0;
//        for (int i = 0; i < s1ap_length - 4; i++) {
//            if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x08) {
//                indx = i;
//                break;
//            }
//        }

//        uint16_t protocol_field2 = htons( *(uint16_t*)&s1ap_data[indx] );
//        //        std::cout << "protocol_field: " << protocol_field << std::endl;

//        if (protocol_field2 == 0x0008) {
//            const int len_indx = indx + (int)(sizeof (uint16_t));
//            uint16_t enb_ue_s1ap_id_lens = htons( *(uint16_t*)&s1ap_data[len_indx] );

//            const int enb_indx = len_indx + (int)(sizeof (uint16_t));
//            uint32_t enb_ue_s1ap_id_ = 0x00;
//            memcpy(&enb_ue_s1ap_id_, &s1ap_data[enb_indx], enb_ue_s1ap_id_lens);


//            if (enb_ue_s1ap_id_lens == 2) {
//                enb_ue_s1ap_id_ = htons(enb_ue_s1ap_id_);
//            } else if (enb_ue_s1ap_id_lens == 4) {
//                enb_ue_s1ap_id_ = htonl(enb_ue_s1ap_id_);
//            }

//            printf("\n FUCK*********** ENB UE S1AP ID: %d\n", enb_ue_s1ap_id_);

//            std::cout << "\n *********** ENB UE S1AP ID: " << enb_ue_s1ap_id  << '\n' << std::endl;

//        }















    }


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
