#ifndef S1AP_PACKET_PARSER_H
#define S1AP_PACKET_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include "socket_mdl.h"
#include "pcap_module.h"

#include "sctp_util.h"
#include "ue_imsi.h"

#include <string.h>



int is_diam_packet(const unsigned char *packet, int length, bool* is_retrans);




#endif /* S1AP_PACKET_PARSER_H */
