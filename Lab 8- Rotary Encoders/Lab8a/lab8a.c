#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"

void writenibble(unsigned char);

int main(void){
	DDRD |= (1 << PD7);	/* Set PD7 for output */
	DDRD |= (1 << PD6);	/* Set PD6 for output */
	DDRD |= (1 << PD5);	/* Set PD5 for output */
	DDRD |= (1 << PD4);	/* Set PD4 for output */
	DDRB |= (1 << PB1); /* Set PB1 for output */
	DDRB |= (1 << PB0); /* Set PB0 for output */
	PORTC |= (1 << PC4); /* Set PC4 for input */
	PORTC |= (1 << PC5); /* Set PC5 for input */
	
	init_lcd();
	writecommand(0x01); //Clears LCD
	writecommand(0x02); //Returns cursor
	int num = 0;
	char A;
	char B;
	stringout("Num:");
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
		_delay_ms(100);
		moveto(4);
		char dispNum[5];
		snprintf(dispNum, 5, " %d ", num);
		stringout(dispNum);

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
    }               // Loop forever
    return 0;   /* never reached */
}

/*
  writenibble - Output four bits from "x" to the display
*/
void writenibble(unsigned char x)
{
	PORTD &= 0b00001111;
	PORTD |= (x & 0b11110000);
	PORTB |= (1 << PB1); // Set E from 0 to 1
	PORTB |= (1 << PB1); // Repeat for delay
	PORTB &= ~(1 << PB1); // Clear E back to 0
}