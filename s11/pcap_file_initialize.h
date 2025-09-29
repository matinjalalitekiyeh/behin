#ifndef PCAP_FILE_INITIALIZE_H
#define PCAP_FILE_INITIALIZE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_NUMBER_OF_ARRAY 1024

typedef struct {
    unsigned long long user_imsi;
    char pcap_file_name[256];
    char prefix[32];
    FILE *file;
} user_t;

int parse_range_from_args(int argc, char *argv[]);
void print_users(void);
size_t get_user_count(void);
user_t *get_users(int idx);
bool is_seperate_user(void);

#endif /* PCAP_FILE_INITIALIZE_H */
