/*******************************************************************************
 *   File Name: codeblock.c
 *
 *     Authors: Chris Stevens - stevechr@onid.oregonstate.edu
 *              Jordan Bayles - bayles.jordan@gmail.com
 *
 * Description: Contains source code for the Microcontroller to interface
 *              with the emitter and receiver blocks
 *
 ******************************************************************************/

/*******************************************************************************
INCLUDES
*******************************************************************************/
#include <stdlib.h>

#define F_CPU 16000000		// cpu speed in hertz
#define TRUE 1
#define FALSE 0
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

/*******************************************************************************
GLOBAL VARIABLES
*******************************************************************************/
uint8_t adc_val0;
uint8_t adc_val1;
uint8_t adc_val2;
uint8_t adc_val3;
uint8_t adc_val4;
uint8_t adc_val5;
uint8_t adc_val6;
uint8_t adc_val7;
uint8_t adc_val[8];
uint16_t tick = 0;
uint8_t blocked = 0x00;
uint8_t i;
uint8_t j = 6;
char *data = "00000000";
char *data2 = "00000000";
uint8_t flag = FALSE;
uint8_t thresh = 154;

/*******************************************************************************
INTERRUPT SERVICE ROUTINES
*******************************************************************************/
ISR(TIMER0_OVF_vect)
{
	tick++;
	if (tick >= 78) {
		// indent exception: indent incorrectly handles binary numbers.
		PORTC ^= 0b00000010;
		tick = 0;
		flag = TRUE;
	}
}

/*******************************************************************************
FUNCTIONS
*******************************************************************************/
void init_adc()
{
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADFR);
	ADCSRA |= (1 << ADEN);
}

void usart_init()
{
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0H = (103) >> 8;
	UBRR0L = 103;
}

void check_adc()
{
	// indent exception: indent incorrectly handles binary numbers.
	PORTC |= 0b00000001;

	//ADC_0
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[0] = ADCH;
	while (!(ADCSRA & (1 << ADSC)));

	//ADC_1
	ADMUX = (1 << REFS0) | (1 << ADLAR) | 1;
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[1] = ADCH;
	while (!(ADCSRA & (1 << ADSC)));

	//ADC_2
	ADMUX = (1 << REFS0) | (1 << ADLAR) | 2;
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[2] = ADCH;
	while (!(ADCSRA & (1 << ADSC)));

	//ADC_3
	ADMUX = (1 << REFS0) | (1 << ADLAR) | 3;
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[3] = ADCH;
	while (!(ADCSRA & (1 << ADSC)));

	//ADC_4
	ADMUX = (1 << REFS0) | (1 << ADLAR) | 4;
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[4] = ADCH;
	while (!(ADCSRA & (1 << ADSC)));

	//ADC_5
	ADMUX = (1 << REFS0) | (1 << ADLAR) | 5;
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[5] = ADCH;
	while (!(ADCSRA & (1 << ADSC)));

	//ADC_6
	ADMUX = (1 << REFS0) | (1 << ADLAR) | 6;
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[6] = ADCH;
	while (!(ADCSRA & (1 << ADSC)));

	//ADC_7
	ADMUX = (1 << REFS0) | (1 << ADLAR) | 7;
	ADCSRA |= (1 << ADSC);
	_delay_us(500);
	adc_val[7] = ADCH;
	while (!(ADCSRA & (1 << ADSC))) {
	};

	// indent exception: indent incorrectly handles binary numbers.
	PORTC &= ~0b00000001;
}

void send_data()
{
	// indent exception: indent incorrectly handles binary numbers.
	PORTC |= 0b10000000;

	for (j = 0; j < 8; ++j) {
		if (adc_val[j] < thresh) {
			blocked |= 1 << j;
		} else if (adc_val[j] >= thresh) {
			blocked &= ~(1 << j);
		}
	}

	itoa(blocked, data, 2);
	for (i = 0; i <= strlen(data); ++i) {
		while (!(UCSR0A & (1 << UDRE0))) {
		};
		UDR0 = data[i];
	}

	// send newline
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = '\n';
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = '\r';

	// indent exception: indent incorrectly handles binary numbers.
	PORTC &= ~(0b10000000);

}

void set_thresh()
{
	// set the threshold to one half of the ambient to maximum range
	check_adc();
	thresh = (255 + adc_val[0]) / 2;
}


int main(int argc, char **argv)
{
	// Initialize the relevant ports
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRF = 0x00;
	PORTF = 0x00;
	DDRD = 0x00;
	PORTC = 0x00;

	TCCR0 = (1 << CS01);
	TIMSK = (1 << TOIE0);

	init_adc();
	usart_init();
	sei();

	set_thresh();
	while (1) {
		check_adc();
		send_data();
	}
}
