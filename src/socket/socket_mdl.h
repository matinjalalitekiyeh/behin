#ifndef SOCKET_MODULE_H
#define SOCKET_MODULE_H

#include <sys/socket.h>
#include <netinet/ip.h>
#include <linux/if_ether.h>

#include <pthread.h>

#define BUFFER_SIZE 65536

#define SOCKET_MDL_ERROR_MSG_LEN 256

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef enum {
    SOCKET_SUCCESS = 0,
    SOCKET_ERROR_INVALID_PARAM = -1,
    SOCKET_ERROR_MEMORY = -2,
    SOCKET_ERROR_IO = -3,
    SOCKET_ERROR_TIMEOUT = -4,
    SOCKET_ERROR_NOT_READY = -5,
} sock_res_t;

typedef struct {//__attribute__((packed))
    pthread_mutex_t lock;
    int sockfd;
    u32 timeout_sec;

    u8 *buffer;
    ssize_t buffer_size;
    char last_error[SOCKET_MDL_ERROR_MSG_LEN];
} sock_context_t;


// Socket functions
int socket_create_raw();
void socket_set_timeout(int sockfd, int seconds);
int socket_receive(int sockfd, unsigned char *buffer, int buffer_size, struct sockaddr *saddr, socklen_t *saddr_size);
void socket_close(int sockfd);

#endif // SOCKET_MODULE_H
