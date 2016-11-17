#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1 << PB0);		/* Set PB0 for output */
	PORTD |= (1 << PD2);	/* Set PD2 for input */
	PORTD |= (1 << PD3);	/* Set PD3 for input */
	PORTD |= (1 << PD4);	/* Set PD4 for input */

	
    while(1) {
		if((PIND & (1 << PD2)) == 0){
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(300);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(300);
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(300);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(300);
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(900);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(900);
		}
		if((PIND & (1 << PD3)) == 0){
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(300);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(300);
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(300);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(300);
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(300);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(900);
		}
	
		if((PIND & (1 << PD4)) == 0){
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(900);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(300);
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(300);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(300);
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(900);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(300);
			PORTB |= (1 << PB0);	/* Output high */
			_delay_ms(300);
			PORTB &= ~(1 << PB0);	/* Output low */
			_delay_ms(900);
		}
    }

    return 0;   /* never reached */
}
