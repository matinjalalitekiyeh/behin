#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include "diameter_packet_parser.h"

#define OUTPUT_FILE "aaa.pcap"

#define TARGET_DIAMETER_PORT 8585  // S1AP default port


int main() {
    sock_context_t *sock = (sock_context_t*)malloc(sizeof(sock_context_t));

    sock->buffer = (u8*)malloc(BUFFER_SIZE * sizeof(char));
    pthread_mutex_init(&sock->lock, NULL);

//    int sock_raw;
    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);
//    unsigned char buffer[BUFFER_SIZE];

    pcap_init(OUTPUT_FILE);

    sock->sockfd = socket_create_raw();
    if (sock->sockfd < 0) {
        pcap_close();
        exit(EXIT_FAILURE);
    }



    socket_set_timeout(sock->sockfd, 1);


    while (1) {
        sock->buffer_size = socket_receive(sock->sockfd, sock->buffer, BUFFER_SIZE, &saddr, &saddr_size);
        if (sock->buffer_size < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
            perror("Recvfrom error");
            break;
        }

        bool is_retrans = false;
        if (is_diam_packet(sock->buffer, sock->buffer_size, &is_retrans)) {
            if (!is_retrans) {
                pcap_write_packet(sock->buffer, (size_t)sock->buffer_size);
            }
        }
    }

    socket_close(sock->sockfd);
    pcap_close();
    return 0;
}
