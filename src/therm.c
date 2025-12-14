#include <avr/io.h>                   
#include <util/delay.h>

#define TEMPERATURE_SAMPLES 10

// ADC1 on PB2
// Clock on PB3
// Reset on PB4

void show_temp(uint8_t t) {
	// Trigger reset of 7-segment displays
	PORTB |= (1<<PB4);
	PORTB &= ~(1<<PB4);

	// Clock the displays up to the temperature value.
	for (uint8_t i = 0; i<t; i++) {
		PORTB |= (1<<PB3);
		PORTB &= ~(1<<PB3);
	}
}


uint8_t read_temp() {
		ADCSRA |= (1<<ADSC);
		while (ADCSRA & (1<<ADSC));

		uint8_t adc_read = ADCH; // 8 bit read, 1 = 10mV
		uint8_t temperature = adc_read-50; // 500mV = 0 deg C
		return temperature;
}


int main(void) {
	DDRB = (1<<DDB3) | (1<<DDB4);

	ADMUX |= (1<<REFS2) | (1<<REFS1); // 2.56v ref, no cap
	ADMUX |= (1<<MUX0); // ADC on PB2 / ADC1
	ADMUX |= (1<<ADLAR); // Left shift to ADCH only
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0); // Prescaler 8, ADC enabled

	
	while (1) {
		uint16_t accumulated_t = 0;

		for (uint8_t i = 0; i < TEMPERATURE_SAMPLES; i++) {
			accumulated_t += read_temp();
			_delay_ms(1000);
		}

		uint8_t avg_t = accumulated_t / TEMPERATURE_SAMPLES;
		show_temp(avg_t);
	}
}
