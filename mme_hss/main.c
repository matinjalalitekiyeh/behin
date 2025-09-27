#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include "diameter_packet_parser.h"

#define OUTPUT_FILE "aaa.pcap"

#include "pcap_file_initialize.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        return EXIT_FAILURE;
    }

    if (!parse_range_from_args(argc, argv)) {
        return EXIT_FAILURE;
    }

    size_t user_cnt = get_user_count();
    for (size_t i = 0; i < user_cnt; i++) {
        const user_t *user = get_users((int)i);
        if (NULL == user)
            continue;
        printf("mtn User %zu: IMSI=%llu, File=%s\n",
               i, user->user_imsi, user->pcap_file_name);
    }

    bool is_seperate_pcap_file = is_seperate_user();
    if (is_seperate_pcap_file) {
        for (size_t i = 0; i < user_cnt; i++) {
            user_t *user = get_users((int)i);
            if (NULL == user)
                continue;
            user->file = pcap_init(user->pcap_file_name);
        }
    } else {
        user_t *user = get_users(0);
        snprintf(user->pcap_file_name, sizeof(user->pcap_file_name),
                     "%s.pcap", user->prefix);
        user->file = pcap_init(user->pcap_file_name);
    }


    sock_context_t *sock = NULL;
    sock_res_t res = socket_context_create(&sock);

    if (SOCKET_SUCCESS != res) {
        printf("Socket creation failed! (%d)", (int)res);
        socket_context_destroy(sock);
        exit(EXIT_FAILURE);
    }

    res = socket_create_raw(sock);
    if (SOCKET_SUCCESS != res) {
        for (size_t i = 0; i < user_cnt; i++) {
            const user_t *user = get_users((int)i);
            if (NULL == user)
                continue;
            if (NULL == user->file)
                continue;
            pcap_close(user->file);
        }
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
                for (size_t i = 0; i < user_cnt; i++) {
                    const user_t *user = get_users((int)i);
                    if (NULL == user)
                        continue;
                    if (NULL == user->file)
                        continue;
                    pcap_write_packet(user->file, sock->buffer, (size_t)sock->buffer_size);
                }
            }
        }
    }

    socket_context_destroy(sock);
    for (size_t i = 0; i < user_cnt; i++) {
        const user_t *user = get_users((int)i);
        if (NULL == user)
            continue;
        if (NULL == user->file)
            continue;
        pcap_close(user->file);
    }
    return 0;
}
