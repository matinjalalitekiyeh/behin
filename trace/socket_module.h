#ifndef SOCKET_MODULE_H
#define SOCKET_MODULE_H

#include <sys/socket.h>
#include <netinet/ip.h>
#include <linux/if_ether.h>

#define BUFFER_SIZE 65536

// Socket functions
int socket_create_raw();
void socket_set_timeout(int sockfd, int seconds);
int socket_receive(int sockfd, unsigned char *buffer, int buffer_size, struct sockaddr *saddr, socklen_t *saddr_size);
void socket_close(int sockfd);

#endif // SOCKET_MODULE_H