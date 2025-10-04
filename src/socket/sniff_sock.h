#ifndef SOCKET_MODULE_H
#define SOCKET_MODULE_H

#include "sock_context.h"

/**
 * socket_context_create - Create socket context structure
 * @ctx: pointer to context pointer for storing allocated context
 *
 * Return: SOCKET_SUCCESS on success, error code on failure.
 *
 * Allocates and initializes socket context with mutex, buffer, and default values.
 * Context must be destroyed with socket_context_destroy() to avoid leaks.
 */
sock_res_t socket_context_create(sock_context_t **ctx);

/**
 * socket_context_destroy - Destroy socket context structure
 * @ctx: pointer to context to destroy
 *
 * Frees all resources associated with socket context.
 * Safe to call with NULL context.
 */
void socket_context_destroy(sock_context_t *ctx);

/**
 * socket_create_raw() - Create a raw socket for packet capture
 * @ctx: Socket context containing configuration parameters
 *
 * This function creates a raw socket at the packet level (AF_PACKET) capable
 * of capturing all Ethernet frames (ETH_P_ALL). The socket is configured with
 * a receive timeout and optional promiscuous mode for comprehensive network
 * monitoring.
 *
 * The function handles proper resource cleanup by closing any existing socket
 * before creating a new one. Context locking ensures thread-safe operation
 * during socket configuration.
 *
 * Context: May sleep due to mutex operations. Should not be called from
 *          interrupt context.
 * Return:
 *   - SOCKET_SUCCESS: Socket created and configured successfully
 *   - SOCKET_ERROR_INVALID_PARAM: Invalid context pointer
 *   - SOCKET_ERROR_IO: Socket creation or configuration failed
 *   - Other socket error codes from lock_context()
 */
sock_res_t socket_create_raw(sock_context_t *ctx);

/**
 * socket_receive_packet() - Receive a packet from the raw socket
 * @ctx: Socket context with active raw socket
 * @data_len: Output parameter for received data length
 *
 * Receives a single packet from the raw socket without holding the context
 * lock, allowing for concurrent socket shutdown operations. Handles various
 * error conditions including timeouts and interruptions.
 *
 * The function checks socket readiness before attempting reception and
 * provides detailed error reporting for different failure scenarios.
 *
 * Context: Does not lock the context, safe for concurrent shutdown
 * Return:
 *   - SOCKET_SUCCESS: Packet received successfully, @data_len contains size
 *   - SOCKET_ERROR_INVALID_PARAM: Invalid parameters
 *   - SOCKET_ERROR_NOT_READY: Socket not properly initialized
 *   - SOCKET_ERROR_TIMEOUT: Receive operation timed out
 *   - SOCKET_ERROR_IO: I/O error or signal interruption
 */
sock_res_t socket_receive_packet(sock_context_t *ctx, ssize_t *data_len);

#endif /* SOCKET_MODULE_H */
