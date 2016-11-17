#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Global Variables
volatile int num;
volatile int lastNum;
volatile char A;
volatile char B;

int main(void) {
	DDRD |= (1 << PD6);	/* Set PD6 for output */
	PORTC |= (1 << PC4); /* Set PC4 for input */
	PORTC |= (1 << PC5); /* Set PC5 for input */
	
	PCICR |= (1 << PCIE1);
	PCMSK1 |= ((1 << PCINT12)| (1 << PCINT13));
	sei();
	
	init_pwm();
	num = OCR0A;
	lastNum = OCR0A;
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
		if(num > 255){
			num = 0;
		}
		else if(num < 0){
			num = 255;
		}
		if(num != lastNum){
			OCR0A = num;
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
	TCCR0A |= (0b11 << WGM00); // Fast PWM mode, modulus = 256
	TCCR0A |= (0b10 << COM0A0); // Turn D6 on at 0x00 and off at OCR0A
	OCR0A = 128; // Initial pulse duty cycle of 50%
	TCCR0B |= (0b101 << CS00); // Prescaler = 1024 => 16ms period
}