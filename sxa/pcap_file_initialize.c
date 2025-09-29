#include "pcap_file_initialize.h"

static user_t users[MAX_NUMBER_OF_ARRAY];
static size_t user_count = 0;
static bool is_seperate = false;

// Adds a user with given IMSI and prefix. Returns 1 if success, 0 if array full.
int add_user(unsigned long long imsi, const char *prefix) {
    if (user_count >= MAX_NUMBER_OF_ARRAY) {
        return 0; // Array full
    }

    users[user_count].user_imsi = imsi;

    // Copy prefix or set default
    if (prefix && prefix[0] != '\0') {
        strncpy(users[user_count].prefix, prefix, sizeof(users[user_count].prefix) - 1);
        users[user_count].prefix[sizeof(users[user_count].prefix) - 1] = '\0';
    } else {
        strcpy(users[user_count].prefix, "s11");
    }

    // Create the pcap filename "<prefix>_<imsi>.pcap"
    snprintf(users[user_count].pcap_file_name,
             sizeof(users[user_count].pcap_file_name),
             "%s_%llu.pcap", users[user_count].prefix, imsi);

    user_count++;
    return 1;
}

// Prints all users
void print_users() {
    printf("Registered users:\n");
    for (size_t i = 0; i < user_count; i++) {
        printf("User %zu: IMSI=%llu, File=%s\n",
               i, users[i].user_imsi, users[i].pcap_file_name);
    }
}

// Parses range string in format start~end and populates users
// Returns 1 on success, 0 on failure
int parse_range_from_args(int argc, char *argv[]) {
    char *range_str = NULL;

    // Find the first non-option argument (not starting with -)
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            range_str = argv[i];
            break;
        } else if (strcmp(argv[i], "--seperate") == 0) {
//            printf("seperate\n");
            is_seperate = true;
        }
        // Ignore other flags
    }

    if (!range_str) {
        fprintf(stderr, "Error: No range specified. Usage: %s start~end\n", argv[0]);
        fprintf(stderr, "Example: %s 999990123456780~999990123456790\n", argv[0]);
        return 0;
    }

    // Find the '~' delimiter
    char *tilde_pos = strchr(range_str, '~');
    if (!tilde_pos) {
        fprintf(stderr, "Error: Invalid range format. Expected start~end\n");
        return 0;
    }

    // Temporarily split the range string
    *tilde_pos = '\0';
    char *start_str = range_str;
    char *end_str = tilde_pos + 1;

    // Convert start_str to unsigned long long with error checking
    errno = 0;
    char *endptr = NULL;
    unsigned long long start_num = strtoull(start_str, &endptr, 10);
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid start number: %s\n", start_str);
        *tilde_pos = '~'; // Restore original string
        return 0;
    }

    // Convert end_str to unsigned long long with error checking
    errno = 0;
    endptr = NULL;
    unsigned long long end_num = strtoull(end_str, &endptr, 10);
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid end number: %s\n", end_str);
        *tilde_pos = '~'; // Restore original string
        return 0;
    }

    *tilde_pos = '~'; // Restore original string

    if (start_num > end_num) {
        fprintf(stderr, "Error: Start number (%llu) cannot be greater than end number (%llu)\n",
                start_num, end_num);
        return 0;
    }

    unsigned long long range_count = end_num - start_num + 1;
    if (range_count > MAX_NUMBER_OF_ARRAY) {
        fprintf(stderr, "Error: Range too large. Max allowed is %d but requested %llu\n",
                MAX_NUMBER_OF_ARRAY, range_count);
        return 0;
    }

    // Add users
    for (unsigned long long i = 0; i < range_count; i++) {
        if (!add_user(start_num + i, "s11")) {
            fprintf(stderr, "Warning: User array full after adding %llu users\n", i);
            break;
        }
    }

    printf("Successfully added %llu users\n", range_count);
    return 1;
}

size_t get_user_count() {
    return user_count;
}

user_t *get_users(int idx) {
    if (idx < 0 || idx >= user_count) {
        return NULL;
    }

    return &users[idx];
}

bool is_seperate_user()
{
    return is_seperate;
}
