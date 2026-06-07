#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>

#include "seven_segment.h"

int main(int argc, char** argv) {
    char first_digit = ' ';
    char second_digit = ' ';

    for (uint16_t voltage = 350; voltage <= 710; voltage+=10) {
        millivolts_to_temp(voltage, &first_digit, &second_digit);
    
        uint8_t first_digit_mask = segments(first_digit);
        uint8_t second_digit_mask = segments(second_digit);

        printf("Input: %i, tens digit %c (0x%02X), ones digit %c (0x%02X)\n",
            voltage, first_digit, first_digit_mask, 
            second_digit, second_digit_mask);
    }
    
    
    return 0;
}
