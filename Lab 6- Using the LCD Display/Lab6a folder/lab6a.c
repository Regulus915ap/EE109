#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void init_lcd(void);
void stringout(char *);
void moveto(unsigned char);

void writecommand(unsigned char);
void writedata(unsigned char);
void writenibble(unsigned char);

int main(void) {
	DDRD |= (1 << PD7);	/* Set PD7 for output */
	DDRD |= (1 << PD6);	/* Set PD6 for output */
	DDRD |= (1 << PD5);	/* Set PD5 for output */
	DDRD |= (1 << PD4);	/* Set PD4 for output */
	DDRB |= (1 << PB1); /* Set PB1 for output */
	DDRB |= (1 << PB0); /* Set PB0 for output */
	
	init_lcd();
	writecommand(0x01); //Clears LCD
	writecommand(0x02); //Returns cursor
	char name[15];
	snprintf(name, 15, "Andrew Prajogi");
	stringout(name);
	
	moveto(16);
	char date[8];
	snprintf(date, 8, "June 24");
	stringout(date);

    while (1) { }               // Loop forever

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
	if(pos > 15){
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
