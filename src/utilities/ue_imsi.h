#ifndef UE_IMSI_H
#define UE_IMSI_H

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

// Define the ED macros for structure layout
#define ED2(x1, x2) x2 x1
#define ED3(x1, x2, x3) x3 x2 x1

typedef struct __attribute__((packed)) {
    ED3(uint8_t digit1:4;,
        uint8_t odd_even:1;,
    uint8_t type:3;)
    ED2(uint8_t digit3:4;,
        uint8_t digit2:4;)
    ED2(uint8_t digit5:4;,
        uint8_t digit4:4;)
    ED2(uint8_t digit7:4;,
        uint8_t digit6:4;)
    ED2(uint8_t digit9:4;,
        uint8_t digit8:4;)
    ED2(uint8_t digit11:4;,
        uint8_t digit10:4;)
    ED2(uint8_t digit13:4;,
        uint8_t digit12:4;)
    ED2(uint8_t digit15:4;,
        uint8_t digit14:4;)
} imsi_structure_t;

int imsi_to_string(const unsigned char* packet_bytes, char* output_buffer, size_t buffer_size);


void parse_imsi_simple(const uint8_t *imsi_data, int length, char *imsi_str);


long long imsi_direct_to_long_long(const unsigned char* packet_bytes);
//{
//    imsi_structure_t* imsi = (imsi_structure_t*)packet_bytes;

//    long long result = 0;
//    int digit_count = 15; // Start with max digits

//    // Handle odd/even length
//    if (!imsi->odd_even) {
//        digit_count = 14;
//    }

//    // std::string imsi_str("999999999999999"); int itr = 15;

//    // Build the number digit by digit
//    long long multiplier = 1;
//    for (int i = digit_count - 1; i >= 0; i--) {
//        uint8_t digit;
//        switch (i) {
//        case 0: digit = imsi->digit1; break;
//        case 1: digit = imsi->digit2; break;
//        case 2: digit = imsi->digit3; break;
//        case 3: digit = imsi->digit4; break;
//        case 4: digit = imsi->digit5; break;
//        case 5: digit = imsi->digit6; break;
//        case 6: digit = imsi->digit7; break;
//        case 7: digit = imsi->digit8; break;
//        case 8: digit = imsi->digit9; break;
//        case 9: digit = imsi->digit10; break;
//        case 10: digit = imsi->digit11; break;
//        case 11: digit = imsi->digit12; break;
//        case 12: digit = imsi->digit13; break;
//        case 13: digit = imsi->digit14; break;
//        case 14: digit = imsi->digit15; break;
//        default: digit = 0;
//        }
//        // imsi_str.at(itr--) = digit;
//        // printf("%d", digit);
//        result += digit * multiplier;
//        multiplier *= 10;
//    }
//    // printf("\n\n");
//    // printf("%s", imsi_str.data());

//    return result;
//}
void long_long_to_imsi_packet(long long imsi_value, unsigned char *packet);


#endif /* UE_IMSI_H */
