#ifndef SOCK_CONTEXT_H
#define SOCK_CONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <linux/if_packet.h>

#define SOCKET_MDL_ERROR_MSG_LEN        256
#define SOCKET_MDL_MAX_BUFFER_SIZE      65536
#define SOCKET_MDL_DEFAULT_TIMEOUT      1
#define SOCKET_MDL_ERROR_MSG_LEN        256

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/**
 * enum sock_res_t - Socket operation result codes
 * @SOCKET_SUCCESS: operation completed successfully
 * @SOCKET_ERROR_INVALID_PARAM: invalid parameter passed
 * @SOCKET_ERROR_MEMORY: memory allocation failure
 * @SOCKET_ERROR_IO: I/O operation failure
 * @SOCKET_ERROR_TIMEOUT: operation timed out
 * @SOCKET_ERROR_INTERNAL: internal error
 *
 * Return codes for socket module functions.
 */
typedef enum {
    SOCKET_SUCCESS = 0,
    SOCKET_ERROR_INVALID_PARAM = -1,
    SOCKET_ERROR_MEMORY = -2,
    SOCKET_ERROR_IO = -3,
    SOCKET_ERROR_TIMEOUT = -4,
    SOCKET_ERROR_NOT_READY = -5,
} sock_res_t;

/**
 * struct sock_context_t - Socket operation context
 * @lock: mutex for thread synchronization
 * @buffer: I/O buffer for socket operations
 * @buffer_size: size of I/O buffer
 * @sockfd: socket file descriptor (-1 if invalid)
 * @timeout_sec: operation timeout in seconds
 * @last_error: buffer for last error message
 *
 * Context for managing socket operations with thread safety and error handling.
 */
typedef struct {
    pthread_mutex_t lock;
    int             sockfd;
    u32             timeout_sec;
    u8*             buffer;
    ssize_t         buffer_size;
    char            last_error[SOCKET_MDL_ERROR_MSG_LEN];
} sock_context_t;

#endif /* SOCK_CONTEXT_H */
