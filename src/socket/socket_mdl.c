#include "socket_mdl.h"

static sock_res_t lock_context(sock_context_t *ctx) {
    if (!ctx) return SOCKET_ERROR_INVALID_PARAM;

    if (pthread_mutex_lock(&ctx->lock) != 0) {
        return SOCKET_ERROR_IO;
    }
    return SOCKET_SUCCESS;
}

static sock_res_t unlock_context(sock_context_t *ctx) {
    if (!ctx) return SOCKET_ERROR_INVALID_PARAM;

    if (pthread_mutex_unlock(&ctx->lock) != 0) {
        return SOCKET_ERROR_IO;
    }
    return SOCKET_SUCCESS;
}

static bool is_socket_ready(const sock_context_t *ctx) {
    return ctx && (ctx->sockfd >= 0);
}

sock_res_t socket_context_create(sock_context_t **ctx)
{
    if (!ctx) {
          return SOCKET_ERROR_INVALID_PARAM;
    }

      sock_context_t *new_ctx = calloc(1, sizeof(*new_ctx));
      if (!new_ctx) {
          return SOCKET_ERROR_MEMORY;
      }

      /* Initialize mutex */
      if (pthread_mutex_init(&new_ctx->lock, NULL) != 0) {
          free(new_ctx);
          return SOCKET_ERROR_IO;
      }

      /* Allocate buffer */
      new_ctx->buffer_size = SOCKET_MDL_MAX_BUFFER_SIZE;
      new_ctx->buffer = (u8*)malloc(SOCKET_MDL_MAX_BUFFER_SIZE);
      if (!new_ctx->buffer) {
          pthread_mutex_destroy(&new_ctx->lock);
          free(new_ctx);
          return SOCKET_ERROR_MEMORY;
      }

      /* Initialize atomic values */
      new_ctx->sockfd = -1;

      new_ctx->timeout_sec = SOCKET_MDL_DEFAULT_TIMEOUT;

      /* Initialize statistics */
      memset(new_ctx->last_error, 0, sizeof(new_ctx->last_error));

      *ctx = new_ctx;
      return SOCKET_SUCCESS;
}

void socket_context_destroy(sock_context_t *ctx)
{
    if (!ctx) {
        return;
    }

    /* Lock to ensure safe cleanup */
    pthread_mutex_lock(&ctx->lock);

    /* Close socket */
    if (ctx->sockfd >= 0) {
        close(ctx->sockfd);
        ctx->sockfd = -1;
    }

    pthread_mutex_unlock(&ctx->lock);
    pthread_mutex_destroy(&ctx->lock);

    if (ctx->buffer) {
        free(ctx->buffer);
        ctx->buffer = NULL;
    }
    if (ctx) {
        free(ctx);
        ctx = NULL;
    }
}

sock_res_t socket_create_raw(sock_context_t *ctx)
{
    if (!ctx)  {
        return SOCKET_ERROR_INVALID_PARAM;
    }

    sock_res_t result = lock_context(ctx);
    if (result != SOCKET_SUCCESS) {
        return result;
    }

    /* Close existing socket */
    if (ctx->sockfd >= 0) {
        close(ctx->sockfd);
        ctx->sockfd = -1;
    }

    /* Create raw socket */
    ctx->sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (ctx->sockfd < 0) {
        printf("Socket creation failed: %s", strerror(errno));
        result = SOCKET_ERROR_IO;
        goto unlock;
    }

    /* Set timeout */
    struct timeval tv = {
        .tv_sec = ctx->timeout_sec,
                .tv_usec = 0
    };

    if (setsockopt(ctx->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        printf("Failed to set socket timeout (fd: %d): %s",
               ctx->sockfd, strerror(errno));
        close(ctx->sockfd);
        ctx->sockfd = -1;
        result = SOCKET_ERROR_IO;
        goto unlock;
    }

unlock:
    unlock_context(ctx);
    return result;
}

sock_res_t socket_receive_packet(sock_context_t *ctx, ssize_t *data_len)
{
    if (!ctx || !data_len) {
        return SOCKET_ERROR_INVALID_PARAM;
    }

    if (!is_socket_ready(ctx)) {
        return SOCKET_ERROR_NOT_READY;
    }

    /* Don't lock during recv to allow concurrent shutdown */
    ssize_t ret = recv(ctx->sockfd, ctx->buffer, (size_t)ctx->buffer_size, 0);

    if (ret < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return SOCKET_ERROR_TIMEOUT;
        }
        if (errno == EINTR) {
            return SOCKET_ERROR_IO; /* Interrupted by signal */
        }

        printf("Receive error: %s", strerror(errno));
        return SOCKET_ERROR_IO;
    }

    *data_len = ret;

    return SOCKET_SUCCESS;
}
