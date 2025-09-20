#ifndef PCAP_MODULE_H
#define PCAP_MODULE_H

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#define SNAP_LEN 1518
#define PCAP_MAGIC 0xa1b2c3d4

typedef struct pcap_hdr_s {
    uint32_t magic_number;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t thiszone;
    uint32_t sigfigs;
    uint32_t snaplen;
    uint32_t network;
} pcap_hdr_t;

typedef struct pcaprec_hdr_s {
    uint32_t ts_sec;
    uint32_t ts_usec;
    uint32_t incl_len;
    uint32_t orig_len;
} pcaprec_hdr_t;

void pcap_init(const char* filename);
void pcap_write_packet(const unsigned char *packet, size_t caplen);
void pcap_write_packet_with_header(const unsigned char *packet, size_t caplen, pcaprec_hdr_t *header);
void pcap_close(void);

#endif // PCAP_MODULE_H
