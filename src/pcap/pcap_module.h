#ifndef PCAP_MODULE_H
#define PCAP_MODULE_H

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

 /**< Maximum snapshot length (Ethernet MTU) */
#define SNAP_LEN 1518

/**< PCAP file format magic number */
#define PCAP_MAGIC 0xa1b2c3d4

/**
 * @brief Global PCAP file header structure
 *
 * This structure represents the header that appears at the beginning
 * of every PCAP file. It describes the file format and capture parameters.
 */
typedef struct pcap_hdr_s {
    uint32_t magic_number;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t thiszone;
    uint32_t sigfigs;
    uint32_t snaplen;
    uint32_t network;
} pcap_hdr_t;

/**
 * @brief Packet record header structure
 *
 * This structure precedes each captured packet in the PCAP file.
 * It contains timing and length information for the packet.
 */
typedef struct pcaprec_hdr_s {
    uint32_t ts_sec;
    uint32_t ts_usec;
    uint32_t incl_len;
    uint32_t orig_len;
} pcaprec_hdr_t;

/**
 * @brief Initialize a new PCAP file
 *
 * Creates a new PCAP file and writes the standard global header.
 * The file is opened in binary write mode.
 *
 * @param[in] filename Path to the PCAP file to create
 * @return FILE pointer to the opened PCAP file on success
 * @return NULL on failure (file cannot be created or written)
 *
 * @note The caller is responsible for closing the file with pcap_close()
 *
 * @code
 * FILE *pcap_file = pcap_init("capture.pcap");
 * if (pcap_file == NULL) {
 *     // Handle error
 * }
 * @endcode
 */
FILE *pcap_init(const char* filename);

/**
 * @brief Write a packet to PCAP file with current timestamp
 *
 * Writes a packet record to the PCAP file using the current system time
 * for the timestamp and automatically generating the packet header.
 *
 * @param[in] pcap_file FILE pointer obtained from pcap_init()
 * @param[in] packet Pointer to the packet data buffer
 * @param[in] caplen Number of bytes to capture from the packet
 *
 * @pre pcap_file must be a valid FILE pointer from pcap_init()
 * @pre packet must point to valid data of at least caplen bytes
 * @pre caplen must not exceed SNAP_LEN
 *
 * @code
 * unsigned char packet_data[100];
 * // ... fill packet_data with packet contents
 * pcap_write_packet(pcap_file, packet_data, sizeof(packet_data));
 * @endcode
 */
void pcap_write_packet(FILE *pcap_file, const unsigned char *packet, size_t caplen);

/**
 * @brief Write a packet to PCAP file with custom header
 *
 * Writes a packet record to the PCAP file using a provided custom header
 * for precise control over timestamp and length fields.
 *
 * @param[in] pcap_file FILE pointer obtained from pcap_init()
 * @param[in] packet Pointer to the packet data buffer
 * @param[in] caplen Number of bytes to capture from the packet
 * @param[in] header Pointer to custom packet header structure
 *
 * @pre pcap_file must be a valid FILE pointer from pcap_init()
 * @pre packet must point to valid data of at least caplen bytes
 * @pre header must point to a valid pcaprec_hdr_t structure
 * @pre header->incl_len should equal caplen
 *
 * @code
 * pcaprec_hdr_t custom_header = {
 *     .ts_sec = 1609459200,
 *     .ts_usec = 500000,
 *     .incl_len = caplen,
 *     .orig_len = original_length
 * };
 * pcap_write_packet_with_header(pcap_file, packet_data, caplen, &custom_header);
 * @endcode
 */
void pcap_write_packet_with_header(FILE *pcap_file, const unsigned char *packet, size_t caplen, pcaprec_hdr_t *header);

/**
 * @brief Close a PCAP file
 *
 * Closes the file stream associated with a PCAP file. This function
 * should be called for every file opened with pcap_init().
 *
 * @param[in] pcap_file FILE pointer to close
 *
 * @pre pcap_file must be a valid FILE pointer from pcap_init()
 *
 * @code
 * pcap_close(pcap_file);
 * @endcode
 */
void pcap_close(FILE *pcap_file);

#endif /* PCAP_MODULE_H */
