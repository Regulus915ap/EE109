#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char num = 0;
char LED[10];

void numDisplay(char num){
	PORTB &= 0b11110000;
	PORTB |= (num >> 3);
	PORTD &= 0b00001111;
	PORTD |= (num << 5);
}

int main(void){
	DDRB |= (1 << PB3);	/* Set PB3 for output */
	DDRB |= (1 << PB2);	/* Set PB2 for output */
	DDRB |= (1 << PB1);	/* Set PB1 for output */
	DDRB |= (1 << PB0);	/* Set PB0 for output */
	DDRD |= (1 << PD7);	/* Set PD7 for output */
	DDRD |= (1 << PD6);	/* Set PD6 for output */
	DDRD |= (1 << PD5);	/* Set PD5 for output */
	
	ADMUX &= 0x00;
	ADMUX |= (1 << MUX1 | 1 << MUX0);
	ADMUX |= (1 << REFS0);
	
	ADCSRA &= 0b01000000;
	ADCSRA |= (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0);
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADIE);
	ADCSRA |= (1 << ADSC);
	sei();
	
	LED[0] = 0b00000001;
	LED[1] = 0b01001111;
	LED[2] = 0b00010010;
	LED[3] = 0b00000110;
	LED[4] = 0b01001100;
	LED[5] = 0b00100100;
	LED[6] = 0b00100000;
	LED[7] = 0b00001111;
	LED[8] = 0b00000000;
	LED[9] = 0b00001100;
	
    while(1) {
    }
	
	return 0;
}

ISR(ADC_vect){
	num = ADC/103;
	numDisplay(LED[num]);
	ADCSRA |= (1 << ADSC);
}