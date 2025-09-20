#include "socket_module.h"
#include <cstdio>
//#include <errno.h>
#include <unistd.h>

int socket_create_raw() {
    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_raw < 0) {
        printf("Socket creation failed");
        return -1;
    }
    return sock_raw;
}

void socket_set_timeout(int sockfd, int seconds) {
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

int socket_receive(int sockfd, unsigned char *buffer, int buffer_size, struct sockaddr *saddr, socklen_t *saddr_size) {
    return recvfrom(sockfd, buffer, buffer_size, 0, saddr, saddr_size);
}

void socket_close(int sockfd) {
    if (sockfd >= 0) {
        close(sockfd);
    }
}