#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include <stdint.h>

uint8_t segments(char c);

void millivolts_to_temp(
        uint16_t mv, 
        char* tens_digit, 
        char* ones_digit
    );

#endif
