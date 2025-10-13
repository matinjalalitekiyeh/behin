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


// Convert long long to packet
void long_long_to_imsi_packet(long long imsi_value, unsigned char *packet)
{
    imsi_structure_t* imsi = (imsi_structure_t*)packet;

    // Clear the packet first
    memset(packet, 0, sizeof(imsi_structure_t));

    // Extract digits from the long long value
    long long temp = imsi_value;
    uint8_t digits[15];
    int digit_count = 0;

    // Extract all digits (in reverse order)
    while (temp > 0 && digit_count < 15) {
        digits[digit_count++] = temp % 10;
        temp /= 10;
    }

    // Set type field (usually 1 for IMSI)
    imsi->type = 1;

    // Set odd_even flag based on digit count
    imsi->odd_even = (digit_count == 15) ? 1 : 0;

    // Assign digits to the packed BCD structure
    // Note: The digits are stored in reverse order in the structure
    if (digit_count >= 1) imsi->digit1 = digits[14];
    if (digit_count >= 2) imsi->digit2 = digits[13];
    if (digit_count >= 3) imsi->digit3 = digits[12];
    if (digit_count >= 4) imsi->digit4 = digits[11];
    if (digit_count >= 5) imsi->digit5 = digits[10];
    if (digit_count >= 6) imsi->digit6 = digits[9];
    if (digit_count >= 7) imsi->digit7 = digits[8];
    if (digit_count >= 8) imsi->digit8 = digits[7];
    if (digit_count >= 9) imsi->digit9 = digits[6];
    if (digit_count >= 10) imsi->digit10 = digits[5];
    if (digit_count >= 11) imsi->digit11 = digits[4];
    if (digit_count >= 12) imsi->digit12 = digits[3];
    if (digit_count >= 13) imsi->digit13 = digits[2];
    if (digit_count >= 14) imsi->digit14 = digits[1];
    if (digit_count >= 15) imsi->digit15 = digits[0];
}
