#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include "diameter_packet_parser.h"

#define OUTPUT_FILE "aaa.pcap"

int parse_range_from_args(int argc, char *argv[], unsigned long long numbers[], size_t max_count, size_t *out_count, char **next_string) {
    if (!out_count || !numbers || max_count == 0) return 0;

    char *range_str = NULL;
    *next_string = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            if (i + 1 < argc) {
                range_str = argv[i + 1];
                break;
            } else {
                return 0;
            }
        } else if (strcmp(argv[i], "--seperate") == 0) {
            printf("seperate\n");
        } else {
            // If it's not a flag and we haven't found -r yet, store as next string
            if (!range_str && !*next_string && argv[i][0] != '-') {
                *next_string = argv[i];
            }
        }
    }

    // If -r was found, process the range
    if (range_str) {
        char *tilde_pos = strchr(range_str, '~');
        if (!tilde_pos) {
            return 0;
        }

        char start_str[64] = {0};
        char end_str[64] = {0};

        // Extract start and end strings
        size_t start_len = tilde_pos - range_str;
        if (start_len >= sizeof(start_str)) return 0;

        strncpy(start_str, range_str, start_len);
        start_str[start_len] = '\0';

        strncpy(end_str, tilde_pos + 1, sizeof(end_str) - 1);
        end_str[sizeof(end_str) - 1] = '\0';

        unsigned long long start_num = strtoull(start_str, NULL, 10);
        unsigned long long end_num = strtoull(end_str, NULL, 10);

        if (start_num > end_num) {
            return 0;
        }

        size_t range_size = end_num - start_num + 1;

        // Check if the range fits in the provided array
        if (range_size > max_count) {
            return 0;
        }

        for (size_t i = 0; i < range_size; i++) {
            numbers[i] = start_num + i;
        }

        *out_count = range_size;
        return 1;
    }
    // If -r was not found but we have a next string
    else if (*next_string) {
        // Try to parse the next string as a single number
        unsigned long long single_num = strtoull(*next_string, NULL, 10);

        if (max_count >= 1) {
            numbers[0] = single_num;
            *out_count = 1;
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // Define a fixed-size array on the stack
    const size_t MAX_NUMBERS = 1000;
    unsigned long long numbers[MAX_NUMBERS];
    size_t count;
    char *next_string = NULL;

    int success = parse_range_from_args(argc, argv, numbers, MAX_NUMBERS, &count, &next_string);

    if (success) {
        printf("Parsed numbers:\n");
        for (size_t i = 0; i < count; i++) {
            printf("%llu\n", numbers[i]);
        }
    } else if (next_string) {
        printf("No range found, next string is: %s\n", next_string);
    } else {
        printf("Failed to parse range and no next string found.\n");
        return 1;
    }


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
