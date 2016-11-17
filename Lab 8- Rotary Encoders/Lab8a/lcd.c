#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

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