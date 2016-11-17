#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Global Variables
volatile int num;
volatile int lastNum;
volatile char A;
volatile char B;

void init_pwm(void);

int main(void) {
	DDRB |= (1 << PB2);	/* Set PB2 for output */
	PORTC |= (1 << PC4); /* Set PC4 for input */
	PORTC |= (1 << PC5); /* Set PC5 for input */
	
	PCICR |= (1 << PCIE1);
	PCMSK1 |= ((1 << PCINT12)| (1 << PCINT13));
	sei();
	
	init_pwm();
	num = OCR1B;
	lastNum = OCR1B;
	if(((PINC & (1 << PC4)) == 0)&&((PINC & (1 << PC5)) == 0)){
		A = 0;
		B = 0;
	}
	if(((PINC & (1 << PC4)) == 0)&&((PINC & (1 << PC5)) != 0)){
		A = 1;
		B = 0;
	}
	if(((PINC & (1 << PC4)) != 0)&&((PINC & (1 << PC5)) == 0)){
		A = 0;
		B = 1;
	}
	if(((PINC & (1 << PC4)) != 0)&&((PINC & (1 << PC5)) != 0)){
		A = 1;
		B = 1;
	}
	
	while (1) {
		if(num > 35){
			num = 35;
		}
		else if(num < 11){
			num = 11;
		}
		if(num != lastNum){
			OCR1B = num;
		}
	}               // Loop forever
    return 0;   /* never reached */
}

ISR(PCINT1_vect){
	lastNum = num;
	
	if(((PINC & (1 << PC4)) == 0)&&((PINC & (1 << PC5)) == 0)&&(A == 0)&&(B == 1)){
		B = 0;
		num += 1;
	}
	if(((PINC & (1 << PC4)) == 0)&&((PINC & (1 << PC5)) == 0)&&(A == 1)&&(B == 0)){
		A = 0;
		num -= 1;
	}
	if(((PINC & (1 << PC4)) == 0)&&((PINC & (1 << PC5)) != 0)&&(A == 0)&&(B == 0)){
		A = 1;
		num += 1;
	}
	if(((PINC & (1 << PC4)) == 0)&&((PINC & (1 << PC5)) != 0)&&(A == 1)&&(B == 1)){
		B = 0;
		num -= 1;
	}
	if(((PINC & (1 << PC4)) != 0)&&((PINC & (1 << PC5)) == 0)&&(A == 0)&&(B == 0)){
		B = 1;
		num -= 1;
	}
	if(((PINC & (1 << PC4)) != 0)&&((PINC & (1 << PC5)) == 0)&&(A == 1)&&(B == 1)){
		A = 0;
		num += 1;
	}
	if(((PINC & (1 << PC4)) != 0)&&((PINC & (1 << PC5)) != 0)&&(A == 0)&&(B == 1)){
		A = 1;
		num -= 1;
	}
	if(((PINC & (1 << PC4)) != 0)&&((PINC & (1 << PC5)) != 0)&&(A == 1)&&(B == 0)){
		B = 1;
		num += 1;
	}
}

void init_pwm(void){
	OCR1A = 312;
	OCR1B = 12;
	TCCR1A |= ((1 << WGM11)|(1 << WGM10)|(1 << COM1B1));
	TCCR1B |= ((1 << WGM13)|(1 << WGM12)|(1 << CS12)|(1 << CS10));
}