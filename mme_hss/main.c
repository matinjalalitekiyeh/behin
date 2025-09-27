#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include "diameter_packet_parser.h"

#define OUTPUT_FILE "aaa.pcap"

int main() {
    sock_context_t *sock = NULL;
    sock_res_t res = socket_context_create(&sock);

    if (SOCKET_SUCCESS != res) {
        printf("Socket creation failed! (%d)", (int)res);
        socket_context_destroy(sock);
        exit(EXIT_FAILURE);
    }

    pcap_init(OUTPUT_FILE);

    res = socket_create_raw(sock);
    if (SOCKET_SUCCESS != res) {
        pcap_close();
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t len = 0;
        res = socket_receive_packet(sock, &len);
        if (SOCKET_SUCCESS != res) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
            perror("Recvfrom error");
            break;
        }

        bool is_retrans = false;
        if (is_diam_packet(sock->buffer, (int)sock->buffer_size, &is_retrans)) {
            if (!is_retrans) {
                pcap_write_packet(sock->buffer, (size_t)sock->buffer_size);
            }
        }
    }

    socket_context_destroy(sock);
    pcap_close();
    return 0;
}
