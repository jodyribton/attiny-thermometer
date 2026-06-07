#include <stdlib.h>
#include <stdint.h>

#include "seven_segment.h"

#define SEG_A (1 << 0)
#define SEG_B (1 << 1)
#define SEG_C (1 << 2)
#define SEG_D (1 << 3)
#define SEG_E (1 << 4)
#define SEG_F (1 << 5)
#define SEG_G (1 << 6)


void millivolts_to_temp(uint16_t mv, char* tens_digit, char* ones_digit) {
    int8_t temperature = (mv + 5) /10 - 50;

    if (temperature < 0) {
        *tens_digit= '-';
    } else if (temperature < 10) {
        *tens_digit = ' ';
    } else {
        *tens_digit = (temperature / 10) + '0';
    }

    if (temperature < -9) {
        *ones_digit = '-';
    } else {
        *ones_digit = abs((temperature % 10)) + '0';
    }
}


uint8_t segments(char c) {
    uint8_t smask = 0;

    switch (c) {
        case '-':
            smask |= (SEG_G);
            break;
        case '0':
            smask |= (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F);
            break;
        case '1':
            smask |= (SEG_B | SEG_C);
            break;
        case '2': 
            smask |= (SEG_A | SEG_B | SEG_G | SEG_E | SEG_D);
            break;
        case '3':
            smask |= (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G);
            break;
        case '4':
            smask |= (SEG_F | SEG_G | SEG_B | SEG_C);
            break;
        case '5':
            smask |= (SEG_A | SEG_F | SEG_G | SEG_C | SEG_D);
            break;
        case '6':
            smask |= (SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_G);
            break;
        case '7':
            smask |= (SEG_A | SEG_B | SEG_C);
            break;
        case '8':
            smask |= (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G);
            break;
        case '9':
            smask |= (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G);
            break;
    }

    return smask;
}

