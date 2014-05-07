// codeblock.c 
// Chris Stevens
// 3.3.14

//Hardware setup:


#define F_CPU 16000000 // cpu speed in hertz 
#define TRUE 1
#define FALSE 0
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

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
char* data = "00000000";
char* data2 = "00000000";
uint8_t flag = FALSE;
uint8_t thresh = 154;

ISR(TIMER0_OVF_vect)
{
	tick++;
	if(tick >= 78){
		PORTC ^= 0b00000010;
		tick = 0;
		flag = TRUE;
	//	check_adc();
	//	send_data();
	}
}


void init_adc(){

ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
ADMUX = (1<<REFS0) | (1<<ADLAR);
ADCSRA |= (1<<ADFR);
ADCSRA |= (1<<ADEN);
//ADCSRA |= (1<<ADSC);

}

void usart_init(){

	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0H = (103) >> 8;
	UBRR0L = 103;

}

void check_adc(){
	PORTC |= 0b00000001;
	//ADC_0
	ADMUX = (1<<REFS0) | (1<<ADLAR);
//	ADMUX = 0b01100000;
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1);
	_delay_us(500);
	adc_val[0] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	//ADC_1
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 1;
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1);
	_delay_us(500);
	adc_val[1] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	//ADC_2
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 2;
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1);
	_delay_us(500);
	adc_val[2] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	//ADC_3
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 3; 
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1);
	_delay_us(500);
	adc_val[3] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	//ADC_4
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 4;
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1);
	_delay_us(500);
	adc_val[4] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	//ADC_5
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 5;
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1);
	_delay_us(500);
	adc_val[5] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	//ADC_6
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 6;
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1);
	_delay_us(500);
	adc_val[6] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	//ADC_7
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 7;
	ADCSRA |= (1<<ADSC);
//	_delay_ms(1); /* asdf */ /*
	_delay_us(500);
	adc_val[7] = ADCH;
	while(!(ADCSRA & (1<<ADSC))){};
	PORTC &= ~0b00000001;
}

void send_data(){

	PORTC |= 0b10000000;

	for(j = 0; j < 8; ++j){
		if(adc_val[j] < thresh){
			blocked |= 1<<j;
		}else if(adc_val[j] >= thresh){
			blocked &= ~(1<<j);
		}
	}
	itoa(blocked, data, 2);
	for(i = 0; i <= strlen(data); ++i){
		while(!(UCSR0A & (1<<UDRE0))){};
		UDR0 = data[i];
	}
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\n';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\r';
	PORTC &= ~(0b10000000);

}

void set_thresh(){
	check_adc();
	thresh = (255+adc_val[0])/2;
}


uint8_t main(){
DDRC = 0xFF;
PORTC = 0x00;
DDRB = 0xFF;
PORTB = 0x00;
DDRF = 0x00;
PORTF = 0x00;
DDRD = 0x00;
PORTC = 0x00;

//TCCR0 = (1<<CS02) | (1<<CS01);
TCCR0 = (1<<CS01);
TIMSK = (1<<TOIE0);

init_adc();
usart_init();
sei();

while(1){

//	ADMUX = (1<<REFS0) | (1<<ADLAR) | (1<<MUX1);
//	ADCSRA |= (1<<ADSC);


/* TEST 1
	data2 = "TEST PHRASE";
	for(i = 0; i < strlen(data2); ++i){
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = data2[i];
	}
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = ';';
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = ' ';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\n';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\r';
*/

/* TEST 2
	check_adc();

	for(j = 0; j < 8; ++j){
		itoa(adc_val[j], data, 10);
		for(i = 0; i < strlen(data); ++i){
			while(!(UCSR0A & (1<<UDRE0))){};
				UDR0 = data[i];
		}
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = ';';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = ' ';
		
	}

		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\n';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\r';
*/

/* TEST 3
	set_thresh();

		itoa(thresh, data, 10);
		for(i = 0; i < strlen(data); ++i){
			while(!(UCSR0A & (1<<UDRE0))){};
				UDR0 = data[i];
		}
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\n';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\r';
*/

// TEST 4
		check_adc();
		send_data();
//
/* TEST 5
		while(PIND != 0b01111111);
		set_thresh();

		while(PIND != 0b10111111);
		check_adc();
		send_data();
*/
		
/*		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '\n';
	for(j = 0; j < 8; ++j){
		data = itoa(thresh[j], data, 10);
		for(i = 0; i <= strlen(data); ++i){
			while(!(UCSR0A & (1<<UDRE0))){};
			UDR0 = data[i];
		}
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = '(';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = j+48;
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = ')';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = ';';
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = ' ';
	}*/
}


}//main
