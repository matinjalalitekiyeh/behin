#include "gtpv2_packet_parser.h"
#include "ue_imsi.h"

static uint32_t previous_tsn = 0;
static bool first_packet = true;

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.1.1"

struct data_t {
    uint8_t msb:4;
    uint8_t lsb:4;
};


struct eps_type_t {
    uint8_t attach_type : 3;
    uint8_t spare_bit : 1;
    uint8_t nas_key_set_identifier : 3;
    uint8_t tsc : 1;
};


uint32_t should_trace_enb_ue_s1ap_id = 0;

void parse_s1ap_message(const uint8_t *s1ap_data, int s1ap_length, const char* direction, bool& is_cap, const unsigned char *packet, int packet_len) {
    if (s1ap_length < 2) return;

    static bool repeater = false;
    static bool capture_tmsi = false;
    static bool valid_imsi = true;
    repeater = !repeater;
    if (!repeater) return;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    static int packet_id = 0;
    uint32_t enb_ue_s1ap_id = 0x00;

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


            memcpy(&enb_ue_s1ap_id, &s1ap_data[enb_idx], enb_ue_s1ap_id_len);


            if (enb_ue_s1ap_id_len == 2) {
                enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
            } else if (enb_ue_s1ap_id_len == 4) {
                enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
            }


            std::cout << "\n *********** ENB UE S1AP ID: " << enb_ue_s1ap_id  << '\n' << std::endl;

        }



        int idx2 = 0;
        for (int i = 0; i < s1ap_length - 3; i++) {
            if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x1a && s1ap_data[i+2] == 0x00) {
                idx2 = i;
                break;
            }
        }

        struct data_t d;
        memset(&d, 0, sizeof (struct data_t));
        memcpy(&d, &s1ap_data[idx2 + 5], sizeof(struct data_t));
        if (d.lsb == 0) {
            //            std::cout << "ok " << std::bitset<4>(d.msb) << ' ' << std::bitset<4>(d.lsb) << std::endl;
        } else if (d.lsb == 1) {
            //            std::cout << "should shift 6 byte " << std::bitset<4>(d.lsb) << std::endl;
            idx2 += 6;
        }


        uint8_t nas_eps_mobility_management_message_type = *(uint8_t*)&s1ap_data[idx2 + 6];
        memcpy(&nas_eps_mobility_management_message_type, &s1ap_data[idx2 + 6], sizeof(uint8_t));
        if (nas_eps_mobility_management_message_type == 0x41) {
            printf("nas_eps_mobility_management_message_type 0x41\n");
//            std::cout << "nas_eps_mobility_management_message_type 0x41" << nas_eps_mobility_management_message_type << std::endl;
        }else if (nas_eps_mobility_management_message_type == 0x48) {
            printf("nas_eps_mobility_management_message_type 0x48\n");
//            std::cout << "nas_eps_mobility_management_message_type 0x48" << nas_eps_mobility_management_message_type << std::endl;
        }

        //7

        uint8_t guti_type = *(uint8_t*)&s1ap_data[idx2 + 9];
        uint8_t type = (guti_type  & 0x07);

        if (type == 6) {
            uint32_t tmsi = 0x00;
            memcpy(&tmsi, &s1ap_data[idx2 + 16], sizeof(uint32_t));
            tmsi = htonl(tmsi);
            printf("guti: %d\n", tmsi);
//            std::cout << "guti: " << tmsi << std::endl;

            if (0x00 == tmsi) {
                should_trace_enb_ue_s1ap_id = enb_ue_s1ap_id;
                is_cap = true;
            }

        } else if (type == 1) {
            std::string imsi_str = imsi_to_string(&s1ap_data[idx2 + 9]);
            std::cout << "imsi: " << imsi_str << std::endl;

            if ("999990123456780" == imsi_str) {
                should_trace_enb_ue_s1ap_id = enb_ue_s1ap_id;
                is_cap = true;
            }

        }

    } else {

        //                auto type = *(uint16_t*)&s1ap_data[7];
        ////                int idx = 0;
        //                if (type == 0) {
        //                    auto enb_ue_s1ap_id_len = *(uint8_t*)&s1ap_data[10];
        //                    uint32_t enb_ue_s1ap_id = 0x00;
        //                    memcpy(&enb_ue_s1ap_id, &s1ap_data[11], enb_ue_s1ap_id_len);
        //                    if (enb_ue_s1ap_id_len == 2) {
        //                        enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
        //                    } else if (enb_ue_s1ap_id_len == 4) {
        //                        enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
        //                    }
        //                    std::cout << "Catch: " << enb_ue_s1ap_id << std::endl;
        ////                    idx = 11 + enb_ue_s1ap_id_len;
        //                }

        //                type = *(uint16_t*)&s1ap_data[idx];
        //                if (type == 8) {
        //                    idx++;
        //                    auto mme_ue_s1ap_id_len = *(uint8_t*)&s1ap_data[idx++];
        //                    uint32_t mme_ue_s1ap_id = 0x00;
        //                    memcpy(&mme_ue_s1ap_id, &s1ap_data[idx], mme_ue_s1ap_id_len);
        //                    if (mme_ue_s1ap_id_len == 2) {
        //                        mme_ue_s1ap_id = htons(mme_ue_s1ap_id);
        //                    } else if (mme_ue_s1ap_id_len == 4) {
        //                        mme_ue_s1ap_id = htonl(mme_ue_s1ap_id);
        //                    }
        //                } //test konammmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm

        uint8_t len = *(uint8_t*)&s1ap_data[10];
        uint32_t enb = 0x00;
        memcpy(&enb, &s1ap_data[11], len);
        if (len == 2) {
            enb = htons(enb);
        } else if (len == 4) {
            enb = htonl(enb);
        }
        uint16_t xxx =  *(uint16_t*)&s1ap_data[11] ;
        xxx = htons(xxx);

        uint16_t xxx2 =  *(uint16_t*)&s1ap_data[17] ;
        xxx2 = htons(xxx2);
        //        std::cout << "Catch: " << enb << ' ' << xxx << ' ' << xxx2 << std::endl;

        uint32_t message_type = 0;//;*(uint32_t*)&s1ap_data[0];

        auto capture = [&]() { if (should_trace_enb_ue_s1ap_id == xxx2) is_cap = true; };

        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x17) {
            printf("DownlinkNASTransport, Identity request: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[1], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
            printf("UplinkNASTransport, Identity response: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x38) {
            printf("DownlinkNASTransport, Authentication request: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
            printf("UplinkNASTransport, Authentication response: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x2c) {
            printf("DownlinkNASTransport, Security mode command: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3d) {
            printf("UplinkNASTransport, Security mode complete: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x1d) {
            printf("DownlinkNASTransport, ESM information request: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3e) {
            printf("UplinkNASTransport, ESM information response: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x80) {
            printf("InitialContextSetupRequest, Attach accept, Activate default EPS bearer context request: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x16 && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x24) {
            printf("UECapabilityInfoIndication, UECapabilityInformation: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x20 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x22) {
            printf("InitialContextSetupResponse, UplinkNASTransport, Attach complete, Activate default EPS bearer context accept: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x45) {
            printf("DownlinkNASTransport, EMM information: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }

    }


    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x17) {
    //        printf("DownlinkNASTransport, Identity request: %d\n", message_type);


    //        int found_at = 0;
    //            for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //                if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                    found_at = i;
    //                    break;
    //                }
    //            }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
    //        printf("UplinkNASTransport, Identity response: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        auto imsi_str = imsi_to_string(&s1ap_data[27]);
    //        if (valid_imsi) {
    //            s_trace_packet_pcap.at(found_at).imsi = imsi_str;
    //        }

    //        valid_imsi  = false;
    //        capture_tmsi = true;

    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x38) {
    //        printf("DownlinkNASTransport, Authentication request: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
    //        printf("UplinkNASTransport, Authentication response: %d\n", message_type);
    //        int index = 4;
    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;


    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x2c) {
    //        int index = 5;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        printf("DownlinkNASTransport, Security mode command: %d\n", message_type);
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3d) {
    //        printf("UplinkNASTransport, Security mode complete: %d\n", message_type);
    //        int index = 6;
    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x1d) {
    //        int index = 7;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        printf("DownlinkNASTransport, ESM information request: %d\n", message_type);
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3e) {
    //        printf("UplinkNASTransport, ESM information response: %d\n", message_type);
    //        int index = 8;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x80) {
    //        printf("InitialContextSetupRequest, Attach accept, Activate default EPS bearer context request: %d\n", message_type);
    //        int index = 9;


    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;


    ////        uint32_t TMSI = ;
    ////        TMSI = htonl(TMSI);
    //        if (capture_tmsi) {
    //            s_trace_packet_pcap.at(found_at).valid_tmsi = htonl(*(uint32_t*)&s1ap_data[115]);
    ////            printf("++++++++++++++++ TMSI %d : %u -> %x %x %x %x\n", capture_tmsi, TMSI, s1ap_data[115], s1ap_data[116], s1ap_data[117], s1ap_data[118]);
    ////            stored_tmsi = TMSI;
    //        }
    //        capture_tmsi = false;
    //        valid_imsi = true;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x16 && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x24) {
    //        printf("UECapabilityInfoIndication, UECapabilityInformation: %d\n", message_type);
    //        int index = 10;


    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;



    //    }

    //    if (s1ap_data[0] == 0x20 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x22) {
    //        printf("InitialContextSetupResponse, UplinkNASTransport, Attach complete, Activate default EPS bearer context accept: %d\n", message_type);
    //        int index = 11;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x45) {
    //        printf("DownlinkNASTransport, EMM information: %d\n", message_type);
    //        int index = 12;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        is_cap = true;

    //    }

    //    //////////////////////////////////////////////////////////////////////////////////////

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3f) {
    //        printf("UplinkNASTransport, Detach request (EPS detach / switch-off): %d\n", message_type);
    //        uint32_t TMSI = *(uint32_t*)&s1ap_data[41];
    //        // TMSI = htonl(TMSI);
    //        printf("++++++++++++++++ TMSI detach %d : \n", TMSI);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x17 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x10) {
    //        printf("UEContextReleaseCommand [NAS-cause=detach]: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //    }

    //    if (s1ap_data[0] == 0x20 && s1ap_data[1] == 0x17 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x0f) {
    //        printf("UEContextReleaseComplete: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;



    ////        std::cout << "should write" << std::endl;

    ////        std::erase(s_trace_packet_pcap, std::distance(std::begin(s_trace_packet_pcap), std::begin(s_trace_packet_pcap) + found_at));

    ////        auto* iter = s_trace_packet_pcap.at(found_at);

    ////        s_trace_packet_pcap.erase(  )




    //    }


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
