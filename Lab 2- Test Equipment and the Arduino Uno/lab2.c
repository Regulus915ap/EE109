#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD |= (1 << DDD7);	/* Set PD7 for output */

    while(1) {
	PORTD &= ~(1 << PD7);	/* Output low */
	_delay_us(5);
	PORTD |= (1 << PD7);	/* Output high */
	_delay_us(15);
	PORTD &= ~(1 << PD7);	/* Output low */
	_delay_us(10);
	PORTD |= (1 << PD7);	/* Output high */
	_delay_us(5);
	PORTD &= ~(1 << PD7);	/* Output low */
	_delay_us(5);
	PORTD |= (1 << PD7);	/* Output high */
	_delay_us(5);
	PORTD &= ~(1 << PD7);	/* Output low */
	_delay_us(5);
    }

    return 0;   /* never reached */
}
