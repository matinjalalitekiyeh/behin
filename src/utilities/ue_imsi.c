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

void parse_imsi_simple(const uint8_t *imsi_data, int length, char *imsi_str) {
//    printf("  Simple IMSI Parsing:\n");

//    char imsi_str[16] = {0};
//    int str_index = 0;

//    // Parse first 7 bytes normally (14 digits)
//    for (int i = 0; i < 7 && i < length; i++) {
//        uint8_t byte = imsi_data[i];
//        uint8_t digit2 = (byte >> 4) & 0x0F;  // Upper nibble first
//        uint8_t digit1 = byte & 0x0F;         // Lower nibble second

//        if (digit2 <= 9) imsi_str[str_index++] = '0' + digit2;
//        if (digit1 <= 9) imsi_str[str_index++] = '0' + digit1;
//    }

//    // Parse last byte (8th byte) with odd/even handling
//    if (length >= 8) {
//        uint8_t last_byte = imsi_data[7];
//        uint8_t digit2 = (last_byte >> 4) & 0x0F;
//        uint8_t digit1 = last_byte & 0x0F;
//        uint8_t odd_even = digit2 & 0x01;  // Odd/even flag is bit 0 of upper nibble

//        if (odd_even) {
//            // Odd: 15 digits total, use only lower nibble
//            if (digit1 <= 9) imsi_str[str_index++] = '0' + digit1;
//        } else {
//            // Even: 14 digits total, use both nibbles
//            if (digit2 <= 9) imsi_str[str_index++] = '0' + digit2;
//            if (digit1 <= 9) imsi_str[str_index++] = '0' + digit1;
//        }
//    }

//    printf("  Simple IMSI: %s\n", imsi_str);

//    printf("  Simple IMSI Parsing:\n");

//      char imsi_str[16] = {0};
      int str_index = 0;

      // Parse first 7 bytes normally (14 digits)
      for (int i = 0; i < 7 && i < length; i++) {
          uint8_t byte = imsi_data[i];
          uint8_t digit1 = byte & 0x0F;         // Lower nibble FIRST
          uint8_t digit2 = (byte >> 4) & 0x0F;  // Upper nibble SECOND

          if (digit1 <= 9) imsi_str[str_index++] = '0' + digit1;
          if (digit2 <= 9) imsi_str[str_index++] = '0' + digit2;
      }

      // Parse last byte (8th byte) with odd/even handling
      if (length >= 8) {
          uint8_t last_byte = imsi_data[7];
          uint8_t digit1 = last_byte & 0x0F;         // Lower nibble
          uint8_t digit2 = (last_byte >> 4) & 0x0F;  // Upper nibble (contains flags)
          uint8_t odd_even = (last_byte >> 4) & 0x01;  // Odd/even flag is bit 0 of upper nibble

//          printf("  Last byte analysis: 0x%02X -> digit1:%d, digit2:%d, odd_even:%d\n",
//                 last_byte, digit1, digit2, odd_even);

          if (odd_even) {
              // Odd: 15 digits total, use only lower nibble
              if (digit1 <= 9) imsi_str[str_index++] = '0' + digit1;
          } else {
              // Even: 14 digits total, use both nibbles
              if (digit1 <= 9) imsi_str[str_index++] = '0' + digit1;
              if (digit2 <= 9) imsi_str[str_index++] = '0' + digit2;
          }
      }

//      printf("  Simple IMSI: %s\n", imsi_str);
}
