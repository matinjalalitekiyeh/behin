#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include "diameter_packet_parser.h"

#define OUTPUT_FILE "aaa.pcap"

#define TARGET_DIAMETER_PORT 8585  // S1AP default port


int main() {
    int sock_raw;
    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);
    unsigned char buffer[BUFFER_SIZE];

    pcap_init(OUTPUT_FILE);

    sock_raw = socket_create_raw();
    if (sock_raw < 0) {
        pcap_close();
        exit(EXIT_FAILURE);
    }

    socket_set_timeout(sock_raw, 1);


    while (1) {
        int data_size = socket_receive(sock_raw, buffer, BUFFER_SIZE, &saddr, &saddr_size);
        if (data_size < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
            perror("Recvfrom error");
            break;
        }

        bool is_retrans = false;
        if (is_diam_packet(buffer, data_size, &is_retrans)) {
            if (!is_retrans) {
                pcap_write_packet(buffer, (size_t)data_size);
            }
        }
    }

    socket_close(sock_raw);
    pcap_close();
    return 0;
}
