#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void numDisplay(char num){
	PORTB &= 0b11110000;
	PORTB |= (num >> 3);
	PORTD &= 0b00001111;
	PORTD |= (num << 5);
}

int main(void)
{
	DDRB |= (1 << PB3);	/* Set PB3 for output */
	DDRB |= (1 << PB2);	/* Set PB2 for output */
	DDRB |= (1 << PB1);	/* Set PB1 for output */
	DDRB |= (1 << PB0);	/* Set PB0 for output */
	DDRD |= (1 << PD7);	/* Set PD7 for output */
	DDRD |= (1 << PD6);	/* Set PD6 for output */
	DDRD |= (1 << PD5);	/* Set PD5 for output */
	PORTC |= (1 << PC2); /* Set PC2 for input */
	PORTC |= (1 << PC4); /* Set PC4 for input */
	
	char LED[10];
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
	
	char num = 0;
	numDisplay(LED[num]);
    while(1) {
		if((PINC & (1 << PC2)) == 0){
			while(1){
				if (num > 9){
					num = 0;
					numDisplay(LED[num]);
					_delay_ms(500);
					num = num + 1;

				}
				else if((PINC & (1 << PC4)) == 0){
					break;
				}
				else{
					numDisplay(LED[num]);
					_delay_ms(500);
					num = num + 1;
				}
			}
		}
		else if((PINC & (1 << PC4)) == 0){
			while(1){
				if (num < 0){
					num = 9;
					numDisplay(LED[num]);
					_delay_ms(500);
					num = num - 1;
				}
				else if((PINC & (1 << PC2)) == 0){
					break;
				}
				else{
					numDisplay(LED[num]);
					_delay_ms(500);
					num = num - 1;
				}
			}
		}
    }

    return 0;   /* never reached */
}