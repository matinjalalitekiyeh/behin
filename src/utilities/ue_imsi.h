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


#endif /* UE_IMSI_H */
