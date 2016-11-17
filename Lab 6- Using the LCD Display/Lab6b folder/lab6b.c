#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void init_lcd(void);
void stringout(char *);
void moveto(unsigned char);

void writecommand(unsigned char);
void writedata(unsigned char);
void writenibble(unsigned char);

char x = 0;
unsigned char num;

void buttonControl(char ADC_Value){
	if (ADC_Value == 6){
		writecommand(0x01); //Clears LCD
		writecommand(0x02); //Returns cursor
		x = x + 1;
		char placement[5];
		snprintf(placement, 5, "%d", x);
		stringout(placement);
	}
	else if (ADC_Value == 2){
		writecommand(0x01); //Clears LCD
		writecommand(0x02); //Returns cursor
		x = x + 10;
		char placement[5];
		snprintf(placement, 5, "%d", x);
		stringout(placement);
	}
	else if (ADC_Value == 4){
		writecommand(0x01); //Clears LCD
		writecommand(0x02); //Returns cursor
		x = x - 10;
		char placement[5];
		snprintf(placement, 5, "%d", x);
		stringout(placement);
	}
	else if (ADC_Value == 0){
		writecommand(0x01); //Clears LCD
		writecommand(0x02); //Returns cursor
		x = x - 1;
		char placement[5];
		snprintf(placement, 5, "%d", x);
		stringout(placement);
	}
	else if (ADC_Value == 8){
		writecommand(0x01); //Clears LCD
		writecommand(0x02); //Returns cursor
		x = 0;
		char placement[5];
		snprintf(placement, 5, "%d", x);
		stringout(placement);
	}
	else{}
}

int main(void) {
	DDRD |= (1 << PD7);	/* Set PD7 for output */
	DDRD |= (1 << PD6);	/* Set PD6 for output */
	DDRD |= (1 << PD5);	/* Set PD5 for output */
	DDRD |= (1 << PD4);	/* Set PD4 for output */
	DDRB |= (1 << PB1); /* Set PB1 for output */
	DDRB |= (1 << PB0); /* Set PB0 for output */
	PINC |= (1 << PC0); /* Set PB0 for input */
	
	ADMUX &= 0x00;
	ADMUX |= (1 << REFS0);
	
	ADCSRA &= 0b01000000;
	ADCSRA |= (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0);
	ADCSRA |= (1 << ADEN);
	sei();
	ADCSRA |= (1 << ADIE);
	
	init_lcd();
	writecommand(0x01); //Clears LCD
	writecommand(0x02); //Returns cursor
	char placement[5];
	snprintf(placement, 5, "%d", x);
	stringout(placement);
	ADCSRA |= (1 << ADSC);

	while (1) { 
		
    }               // Loop forever
    return 0;   /* never reached */
}

/*
  init_lcd - Do various things to initialize the LCD display
*/
void init_lcd()
{
    _delay_ms(15);              // Delay at least 15ms

    // Use writenibble to send 0011
	writenibble(0x30);
    _delay_ms(5);               // Delay at least 4msec

    // Use writenibble to send 0011
	writenibble(0x30);
    _delay_us(120);             // Delay at least 100usec

    // Use writenibble to send 0011
	writenibble(0x30);
	
    // Use writenibble to send 0010    // Function Set: 4-bit interface
	writenibble(0x20);

    _delay_ms(2);
    
    writecommand(0x28);         // Function Set: 4-bit interface, 2 lines

    writecommand(0x0f);         // Display and cursor on

}

ISR(ADC_vect){
	_delay_ms(250);
	num = ADC/103;
	buttonControl(num);
	ADCSRA |= (1 << ADSC);
}

/*
  stringout - Print the contents of the character string "str"
  at the current cursor position.
*/
void stringout(char *str)
{
	int count = 0;
	for(; str[count] != 0x00; count++){
		writedata(str[count]);
	}
}

/*
  moveto - Move the cursor to the postion "pos"
*/
void moveto(unsigned char pos)
{
	if(pos > 31){
		pos = 31;
	}
	else if(pos > 15){
		writecommand(0xc0 + pos - 16);
	}
	else{
		writecommand(0x80 + pos);
	}
}

/*
  writecommand - Output a byte to the LCD display instruction register.
*/
void writecommand(unsigned char x)
{
	PORTB &= ~(1 << PB0); //RS=0
	writenibble(x);
	writenibble(x << 4);
	_delay_ms(2);
}

/*
  writedata - Output a byte to the LCD display data register
*/
void writedata(unsigned char x)
{
	PORTB |= (1 << PB0); //RS=1;
	writenibble(x);
	writenibble(x << 4);
	_delay_ms(2);
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