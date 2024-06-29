#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
	DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6); 
	
	PORTC = 0xFF;

	while(1)
	{
		for(int i = 0; i < 6; i++)
		{
				PORTB = (1 << i);
				_delay_ms(5);
		}
	}
	
	return 0;
}