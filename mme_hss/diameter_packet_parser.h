#ifndef S1AP_PACKET_PARSER_H
#define S1AP_PACKET_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include "socket_mdl.h"
#include "pcap_module.h"

#include "sctp_util.h"
#include "ue_imsi.h"


typedef struct {
    uint8_t version;
    uint8_t length[3];       // 3-byte array for length
    uint8_t flags;
    uint8_t command_code[3]; // 3-byte array for command code
    uint32_t application_id;
    uint32_t hop_by_hop_identifier;
    uint32_t end_to_end_identifier;
} diam_header_raw_t;


int is_diam_packet(const unsigned char *packet, int length, bool* is_retrans);




#endif /* S1AP_PACKET_PARSER_H */
