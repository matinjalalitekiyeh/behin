#ifndef SCTP_UTIL_H
#define SCTP_UTIL_H

#include <stdint.h>

typedef struct {
    uint16_t source_port;
    uint16_t dest_port;
    uint32_t verification_tag;
    uint32_t checksum;
} sctp_common_header_t;

typedef struct {
    uint8_t type;
    uint8_t flags;
    uint16_t length;
} sctp_chunk_header_t;

typedef struct sctp_data_chunk {
    sctp_chunk_header_t chunk_header;
    uint32_t tsn;
    uint16_t stream_id;
    uint16_t stream_seq;
    uint32_t protocol_id;
} sctp_data_chunk_t;

#endif /* SCTP_UTIL_H */
