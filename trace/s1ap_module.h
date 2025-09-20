#ifndef S1AP_MODULE_H
#define S1AP_MODULE_H

#include <stdint.h>
#include <map>
#include <set>
#include <string>
#include <cstring>

#include "pcap_module.h"
#include <cstdlib>
#include <vector>

// Configuration - KEEP YOUR ORIGINAL VALUES
#define TARGET_CLIENT_IP "127.0.1.1"
#define TARGET_SERVER_IP "127.0.0.2"
#define TARGET_PORT 36412  // S1AP default port


#define TARGET_DIAMETER_CLIENT_IP "127.0.0.8"
#define TARGET_DIAMETER_SERVER_IP "127.0.0.2"
#define TARGET_DIAMETER_PORT 3868  // S1AP default port
#define SCTP_PPID_DIAM 46


// S1AP Protocol constants
#define SCTP_PPID_S1AP 18  // S1AP uses SCTP Payload Protocol ID 18

// S1AP Protocol constants and structures
#define S1AP_PROTOCOL_IE_PLMN_ID 0x01
#define S1AP_PROTOCOL_IE_GLOBAL_ENB_ID 0x03
#define S1AP_PROTOCOL_IE_MME_GROUP_ID 0x04

// S1AP Message Types
#define S1AP_MSG_S1_SETUP_REQUEST 0x11
#define S1AP_MSG_INITIAL_UE_MESSAGE 0x12
#define S1AP_MSG_DOWNLINK_NAS_TRANSPORT 0x13
#define S1AP_MSG_UPLINK_NAS_TRANSPORT 0x14

// NAS-EPS Message Types
#define NAS_EPS_MSG_ATTACH_REQUEST 0x41
#define NAS_EPS_MSG_IDENTITY_RESPONSE 0x16
#define NAS_EPS_IE_IMSI 0x02

// Define the ED macros for structure layout
#define ED2(x1, x2) x2 x1
#define ED3(x1, x2, x3) x3 x2 x1

// struct packet_data_pcap_t {
//     pcaprec_hdr_t rec_hdr;
//     uint32_t len;
//     uint8_t data[1024 * 64];
//     uint32_t id;
// };
// packet_data_pcap_t m_packet_data_pcap[13];

struct packet_t {
    pcaprec_hdr_t rec_header;
    uint32_t size;
    uint32_t packet_type;
    unsigned char packet[500];
};

struct trace_packet_pcap_t {
    std::string imsi;
    uint32_t count;
    uint16_t ran_id;
    uint16_t ue_id;
    packet_t packet[20];
    uint32_t tmsi;
    uint32_t valid_tmsi;
};



// IMSI structure using ED macros for endianness handling
typedef struct __attribute__((packed)) {
    ED3(uint8_t digit1:4;,
            uint8_t odd_even:1;,
    uint8_t type:3;)
    ED2(uint8_t digit3:4;,
            uint8_t digit2:4;)
    ED2(uint8_t digit5:4;,
            uint8_t digit4:4;)
    ED2(uint8_t digit7:4;,
            uint8_t digit6:4;)
    ED2(uint8_t digit9:4;,
            uint8_t digit8:4;)
    ED2(uint8_t digit11:4;,
            uint8_t digit10:4;)
    ED2(uint8_t digit13:4;,
            uint8_t digit12:4;)
    ED2(uint8_t digit15:4;,
            uint8_t digit14:4;)
} imsi_structure_t;

// PLMN ID structure (3GPP 29.274)
struct plmn_id {
    uint8_t mcc_digit2 : 4;
    uint8_t mcc_digit1 : 4;
    uint8_t mnc_digit3 : 4;
    uint8_t mcc_digit3 : 4;
    uint8_t mnc_digit2 : 4;
    uint8_t mnc_digit1 : 4;
} __attribute__((packed));

// SCTP Common Header
struct sctp_common_header {
    uint16_t source_port;
    uint16_t dest_port;
    uint32_t verification_tag;
    uint32_t checksum;
};

// SCTP Chunk Header
struct sctp_chunk_header {
    uint8_t type;
    uint8_t flags;
    uint16_t length;
};

// SCTP Data Chunk
struct sctp_data_chunk {
    struct sctp_chunk_header chunk_header;
    uint32_t tsn;
    uint16_t stream_id;
    uint16_t stream_seq;
    uint32_t protocol_id;
};

// Structure to track SCTP connection state and seen TSNs
struct sctp_connection_state {
    std::set<uint32_t> seen_tsn_client;  // TSNs seen from client
    std::set<uint32_t> seen_tsn_server;  // TSNs seen from server
    uint32_t last_client_tsn;
    uint32_t last_server_tsn;
};

// S1AP functions
long long imsi_direct_to_long_long(const unsigned char* packet_bytes);
std::string get_connection_key(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port);
bool is_retransmission(const std::set<uint32_t>& seen_tsns, uint32_t tsn);
void parse_nas_eps_message(const uint8_t *nas_data, int nas_length, const char* direction);
void parse_plmn_id(const uint8_t *data, int length);
void parse_s1ap_message(const uint8_t *s1ap_data, int s1ap_length, const char* direction, bool& is_cap, const unsigned char *packet, int packet_len);
int is_s1ap_packet(const unsigned char *packet, int length, bool& is_retrans, bool& is_cap);
int is_diameter_packet(const unsigned char *packet, int length, bool& is_retrans, bool& is_cap);

std::vector<trace_packet_pcap_t> &get_packet();

#endif // S1AP_MODULE_H
