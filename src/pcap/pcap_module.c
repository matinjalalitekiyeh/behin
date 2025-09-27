#include "pcap_module.h"
#include <malloc.h>
//static FILE *pcap_file = NULL;

FILE *pcap_init(const char* filename) {
    FILE *pcap_file = fopen(filename, "wb");
    if (!pcap_file) {
        perror("Failed to create pcap file");
    }

    pcap_hdr_t pcap_hdr = {
        .magic_number = PCAP_MAGIC,
        .version_major = 2,
        .version_minor = 4,
        .thiszone = 0,
        .sigfigs = 0,
        .snaplen = SNAP_LEN,
        .network = 1 /* eth */
    };
    fwrite(&pcap_hdr, sizeof(pcap_hdr_t), 1, pcap_file);
    return pcap_file;
}

void pcap_write_packet(FILE *pcap_file, const unsigned char *packet, size_t caplen) {
    if (!pcap_file) return;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    pcaprec_hdr_t rec_hdr = {
        .ts_sec = (uint32_t)(tv.tv_sec),
        .ts_usec = (uint32_t)(tv.tv_usec),
        .incl_len = (uint32_t)(caplen),
        .orig_len = (uint32_t)(caplen)
    };

    fwrite(&rec_hdr, sizeof(pcaprec_hdr_t), 1, pcap_file);
    fwrite(packet, caplen, 1, pcap_file);
    fflush(pcap_file);
}

void pcap_write_packet_with_header(FILE *pcap_file, const unsigned char *packet, size_t caplen, pcaprec_hdr_t* header) {
    if (!pcap_file) return;

    fwrite(header, sizeof(pcaprec_hdr_t), 1, pcap_file);
    fwrite(packet, caplen, 1, pcap_file);
    fflush(pcap_file);
}

void pcap_close(FILE *pcap_file) {
    if (pcap_file) {
        fclose(pcap_file);
        pcap_file = NULL;
    }
}
