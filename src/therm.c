#include <avr/io.h>                   
#include <avr/interrupt.h>
#include <stdint.h>

#include "seven_segment.h"

#define TEMPERATURE_READINGS 10

volatile uint8_t digit_segments[2] = {0x40, 0x40};
volatile uint8_t powered_digit = 0;

volatile uint8_t t_reads = 0;
volatile uint16_t t_sum = 0;

ISR(ADC_vect) {
    // Save temperature reading
    t_reads++;
    t_sum += ADC;

    // When we've accumulated sufficient readings, take the average and update
    // the display.
    if (t_reads == TEMPERATURE_READINGS) {
        uint16_t average_reading = t_sum / TEMPERATURE_READINGS;
        uint16_t millivolts = (average_reading * 1100UL) / 1024;

        char tens_digit = ' ';
        char ones_digit = ' ';
        millivolts_to_temp(millivolts, &tens_digit, &ones_digit); 
        digit_segments[0] = segments(tens_digit);
        digit_segments[1] = segments(ones_digit);

        t_reads = 0;
        t_sum = 0;
    }
}

ISR(TIM1_COMPB_vect) {
	// Toggle debug pin
	PORTB ^= (1 << PB0);
	
	// Start ADC conversion
	ADCSRA |= (1<<ADSC);
}

ISR(TIM0_COMPA_vect ) {
    // Update 7-segment displays
    
    // Turn off the digits
    PORTB &= ~((1<<PB1) | (1<<PB2));

    // Switch to the other digit
    powered_digit ^= 1;

    // Set the segment outputs accordingly
    PORTA = (PORTA & 0x01) | (digit_segments[powered_digit] << 1);

    // Turn on the digit
    if (powered_digit == 0) {
        PORTB |= (1<<PB1);
    } else {
        PORTB |= (1<<PB2);
    }
}

void setup() {
	// ADC0 is the temperature input. Reference voltage is 1.1v.
    ADMUX |= (1<<REFS1);
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN) | (1<<ADIE);
    DIDR0 |= (1<<ADC0D);

    // PA1-PA7 are outputs controlling the multiplexed segments.
    DDRA = (1 << DDA1) | (1<<DDA2) | (1<<DDA3) | (1<<DDA4) | (1<<DDA5) 
        | (1<<DDA6) | (1<<DDA7);

    // PB1 & PB2 are outputs that turn the digits on (high) or off (low).
    // PB0 is a debug output.
    DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2);

	// Timer 0 interrupt every 4ms to multiplex the digits.
    TCCR0B = (1<<CS00) | (1<<CS02); // Timer frequency set to clk/1024
    OCR0A = 3;
    TCCR0A = (1<<WGM01); // Run timer in CTC mode

    // Timer 1 should trigger a temperature read every ~3 seconds.
    OCR1B = 3000;
    OCR1A = 3010;
    TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10); // Start timer with frequency set to clk/1024

    // Enable ADC interrupt.
    TIMSK1 |= (1<<OCIE1B);

    // Enable multiplexing interrupt.
    TIMSK0 |= (1<<OCIE0A);
}

int main() {
    setup();

    sei();
    while(1) {};
}
