#include "ue_imsi.h"
#include <stdlib.h>

int imsi_to_string(const unsigned char* packet_bytes, char* output_buffer, size_t buffer_size) {
    const imsi_structure_t* imsi = (const imsi_structure_t*)packet_bytes;

    int digit_count = imsi->odd_even ? 15 : 14;

    if (buffer_size < (size_t)(digit_count + 1)) {
        return -1;
    }

    char* current_pos = output_buffer;

    *current_pos++ = '0' + imsi->digit1;
    *current_pos++ = '0' + imsi->digit2;
    *current_pos++ = '0' + imsi->digit3;
    *current_pos++ = '0' + imsi->digit4;
    *current_pos++ = '0' + imsi->digit5;
    *current_pos++ = '0' + imsi->digit6;
    *current_pos++ = '0' + imsi->digit7;
    *current_pos++ = '0' + imsi->digit8;
    *current_pos++ = '0' + imsi->digit9;
    *current_pos++ = '0' + imsi->digit10;
    *current_pos++ = '0' + imsi->digit11;
    *current_pos++ = '0' + imsi->digit12;
    *current_pos++ = '0' + imsi->digit13;
    *current_pos++ = '0' + imsi->digit14;

    if (imsi->odd_even) {
        *current_pos++ = '0' + imsi->digit15;
    }

    *current_pos = '\0';

    return digit_count;
}



long long imsi_direct_to_long_long(const unsigned char *packet)
{
    const imsi_structure_t* imsi = (const imsi_structure_t*)packet;

    long long result = 0;
    int digit_count = 15;

    if (!imsi->odd_even) {
        digit_count = 14;
    }
    long long multiplier = 1;
    for (int i = digit_count - 1; i >= 0; i--) {
        uint8_t digit;
        switch (i) {
        case 0: digit = imsi->digit1; break;
        case 1: digit = imsi->digit2; break;
        case 2: digit = imsi->digit3; break;
        case 3: digit = imsi->digit4; break;
        case 4: digit = imsi->digit5; break;
        case 5: digit = imsi->digit6; break;
        case 6: digit = imsi->digit7; break;
        case 7: digit = imsi->digit8; break;
        case 8: digit = imsi->digit9; break;
        case 9: digit = imsi->digit10; break;
        case 10: digit = imsi->digit11; break;
        case 11: digit = imsi->digit12; break;
        case 12: digit = imsi->digit13; break;
        case 13: digit = imsi->digit14; break;
        case 14: digit = imsi->digit15; break;
        default: digit = 0;
        }
        result += digit * multiplier;
        multiplier *= 10;
    }

    return result;
}
