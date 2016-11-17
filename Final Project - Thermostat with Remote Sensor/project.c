#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"
#include "ds1631.h"

void writenibble(unsigned char);
void ds1631_init ( void );
void ds1631_conv ( void );
void ds1631_temp ( unsigned char *);

//Global Variables
volatile int temp = 0;
volatile int tempT = 0;
volatile int lastTemp = 0;
volatile int low = 70;
volatile int lastLow = 70;
volatile int high = 100;
volatile int lastHigh = 100;
volatile int adjHigh = 0;
volatile char A;
volatile char B;
volatile int rTemp = 0;
volatile int lastRTemp = 0;
volatile char retVal[4];
volatile char retVal[4];
volatile int count;

// void rx_char(){
	// // Wait for receive complete flag to go high
	// while ( (count == 4) && !(UCSR0A & (1 << RXC0)) ) {
		// rTemp = retVal[1]*100 + retVal[2]*10 + retVal[3];
		// if(retVal[0] == '-')
			// rTemp *= -1;
	// }
// }

//Transmission Function
void tx_char(char ch){
	// Wait for transmitter data register empty
	while ((UCSR0A & (1<<UDRE0)) == 0) {}
	UDR0 = ch;
}

int main(void) {
	DDRD |= (1 << PD7);	/* Set PD7 for output */
	DDRD |= (1 << PD6);	/* Set PD6 for output */
	DDRD |= (1 << PD5);	/* Set PD5 for output */
	DDRD |= (1 << PD4);	/* Set PD4 for output */
	DDRD |= (1 << PD3); /* Set PD3 for output */
	DDRD |= (1 << PD2); /* Set PD2 for output */
	DDRD |= (1 << PD1); /* Set PD1 for output */
	DDRB |= (1 << PB1); /* Set PB1 for output */
	DDRB |= (1 << PB0); /* Set PB0 for output */
	DDRB |= (1 << PB5); /* Set PB5 for output */
	PORTB &= ~(1 << PB5);
	PORTB |= (1 << PB3); /* Set PB3 for input */
	PORTB |= (1 << PB4); /* Set PB4 for input */
	PORTC |= (1 << PC1); /* Set PC1 for input */
	PORTC |= (1 << PC2); /* Set PC2 for input */
	PORTC |= (1 << PC4); /* Set PC4 for input */
	PORTC |= (1 << PC5); /* Set PC5 for input */
	
	//Set Interrupts
	PCICR |= (1 << PCIE1);
	PCMSK1 |= ((1 << PCINT9)|(1 << PCINT10));
	sei();
	
	//Define MYUBRR
	#define FOSC 16000000 // Clock frequency
	#define BAUD 9600 // Baud rate used
	#define MYUBRR FOSC/16/BAUD-1 // Value for UBRR0
	UBRR0 = MYUBRR; // Set baud rate
	
	UCSR0B |= (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); // Enable RX and TX
	UCSR0C = (3 << UCSZ00); // Async., no parity,
							// 1 stop bit, 8 data bits
	
	unsigned char t [2];
	ds1631_init (); // Initialize the DS1631
	ds1631_conv (); // Set the DS1631 to do conversions
	
	//Write Initial to LCD Display
	init_lcd();
	writecommand(0x01); //Clears LCD
	writecommand(0x02); //Returns cursor
	stringout("Temp:");
	char dispTemp[4];
	snprintf(dispTemp, 4, "%d", temp);
	stringout(dispTemp);
	moveto(9);
	stringout("RMT:");
	char dispRMT[4];
	snprintf(dispRMT, 4, "%d", rTemp);
	stringout(dispRMT);
	moveto(16);
	stringout("Low:");
	char dispLow[4];
	snprintf(dispLow, 4, "%d", low);
	stringout(dispLow);
	moveto(24);
	stringout("High:");
	char dispHigh[4];
	snprintf(dispHigh, 4, "%d", high);
	stringout(dispHigh);
	
	//Set Correct Settings to Rotary Encoder
	if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) == 0)){
		A = 0;
		B = 0;
	}
	if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) != 0)){
		A = 1;
		B = 0;
	}
	if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) == 0)){
		A = 0;
		B = 1;
	}
	if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) != 0)){
		A = 1;
		B = 1;
	}
	
	while (1) {
		ds1631_temp(t); // Read the temperature data
		
		//Convert Local Temperature Data
		tempT = t[0];
		tempT = 90 * tempT;
		if(t[1] != 0){
			tempT+= 45;
		}
		temp = tempT/50 + 32;
		
		//Convert Local Temperature for Transmission
		char tempArr[4];
		if (temp >= 0){
			tempArr[0] = '+';
		}
		else{
			tempArr[0] = '-';
		}
		tempArr[1] = temp/100 + 0x30;
		tempArr[2] = (temp%100)/10 + 0x30;
		tempArr[3] = temp%10 + 0x30;
		
		//Transmit Local Temperature
		int i;
		for(i = 0; i < 4; i++){
			tx_char(tempArr[i]);
		}
		
		//Flag Detection for Low/High Adjustment
		if((PINB & (1 << PB3)) == 0){
			_delay_ms(100);
			adjHigh = 0;
		}
		if((PINB & (1 << PB4)) == 0){
			_delay_ms(100);
			adjHigh = 1;
		}
		
		//LED On/Off Conditions Heater
		if(low > temp){
			PORTD |= (1 << PB3);
		}
		else{
			PORTD &= ~(1 << PB3);
		}
		
		//LED On/Off Conditions Cooler
		if(high < temp){
			PORTD |= (1 << PB2);
		}
		else{
			PORTD &= ~(1 << PB2);
		}
		
		//Update Display when Needed
		if(temp != lastTemp){
			_delay_ms(100);
			moveto(5);
			char dispTemp[5];
			snprintf(dispTemp, 5, "%d ", temp);
			stringout(dispTemp);
			lastTemp = temp;
		}
		if(rTemp != lastRTemp){
			_delay_ms(100);
			moveto(13);
			char dispRTemp[5];
			snprintf(dispRTemp, 5, "%d ", rTemp);
			stringout(dispRTemp);
			lastRTemp = rTemp;
		}
		
		if(adjHigh == 0){
			if(low >= high){
				low = high; //Ensures that Low does not surpass High
			}
			if(low != lastLow){
				_delay_ms(100);
				moveto(20);
				char dispTemp[5];
				snprintf(dispTemp, 5, "%d ", low);
				stringout(dispTemp);
				lastLow = low;
			}
		}
		
		if(adjHigh == 1){
			if(high <= low){
				high = low; //Ensures that High does not go under Low
			}
			if(high != lastHigh){
				_delay_ms(100);
				moveto(29);
				char dispTemp[5];
				snprintf(dispTemp, 5, "%d ", high);
				stringout(dispTemp);
				lastHigh = high;
			}
		}
	}               // Loop forever
    return 0;   /* never reached */
}

ISR(USART_RX_vect){
	// character has been received
	unsigned char receiver = UDR0;
	
	//Translate characters
	if((receiver == '+')||(receiver == '-')){
		retVal[0] = receiver;
		count = 1;
	}
	else{
		retVal[count] = receiver - 0x30;
		count++;
	}
	
	//Compile Characters
	if(count == 4){
		rTemp = retVal[1]*100 + retVal[2]*10 + retVal[3];
		if(retVal[0] == '-'){
			rTemp *= -1;
		}
	}
	
	// Test Transmission
	// writecommand(1);
	// char s[20];
	// snprintf(s,20,"char: %d", receiver);
	// stringout(s);	
	// _delay_ms(1000);
}

//Adjust High and Low Setting
ISR(PCINT1_vect){
	//Adjust Low
	if(adjHigh == 0){
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) == 0)&&(A == 0)&&(B == 1)){
			B = 0;
			low += 1;
		}
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) == 0)&&(A == 1)&&(B == 0)){
			A = 0;
			low -= 1;
		}
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) != 0)&&(A == 0)&&(B == 0)){
			A = 1;
			low += 1;
		}
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) != 0)&&(A == 1)&&(B == 1)){
			B = 0;
			low -= 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) == 0)&&(A == 0)&&(B == 0)){
			B = 1;
			low -= 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) == 0)&&(A == 1)&&(B == 1)){
			A = 0;
			low += 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) != 0)&&(A == 0)&&(B == 1)){
			A = 1;
			low -= 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) != 0)&&(A == 1)&&(B == 0)){
			B = 1;
			low += 1;
		}
	}
	
	//Adjust High
	if(adjHigh == 1){
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) == 0)&&(A == 0)&&(B == 1)){
			B = 0;
			high += 1;
		}
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) == 0)&&(A == 1)&&(B == 0)){
			A = 0;
			high -= 1;
		}
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) != 0)&&(A == 0)&&(B == 0)){
			A = 1;
			high += 1;
		}
		if(((PINC & (1 << PC1)) == 0)&&((PINC & (1 << PC2)) != 0)&&(A == 1)&&(B == 1)){
			B = 0;
			high -= 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) == 0)&&(A == 0)&&(B == 0)){
			B = 1;
			high -= 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) == 0)&&(A == 1)&&(B == 1)){
			A = 0;
			high += 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) != 0)&&(A == 0)&&(B == 1)){
			A = 1;
			high -= 1;
		}
		if(((PINC & (1 << PC1)) != 0)&&((PINC & (1 << PC2)) != 0)&&(A == 1)&&(B == 0)){
			B = 1;
			high += 1;
		}
	}
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