#include <avr/io.h>
#include <util/delay.h>


#define SELECT_DISPLAY(x)  PORTB = (1 << x);
#define SET_DIGIT(x)       PORTC = (x < 0 ? 0 : segments[x]);

int main(void)
{
	// inicjalizacja

	// PB[0-5] jako wyjscia - do wyboru wyswietlacza
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
	
	// PC[0-6] jako wyjscia - sterowanie wyswietlaczem 7-segmentowym
	DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6); 
	
	const uint8_t segments[10] = 
	{ //	a			 b			  c			   d			e			 f			 g
		(1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5)             , // 0
					 (1 << PC1) | (1 << PC2)                                                    , // 1
		(1 << PC0) | (1 << PC1)				 | (1 << PC3) | (1 << PC4) | 			  (1 << PC6), // 2
		(1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) |							  (1 << PC6), // 3
					 (1 << PC1) | (1 << PC2) 			  | (1 << PC5) | (1 << PC6)				, // 4
		(1 << PC0) |			  (1 << PC2) | (1 << PC3) |			     (1 << PC5)	| (1 << PC6), // 5
		(1 << PC0) | 			  (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6), // 6
		(1 << PC0) | (1 << PC1) | (1 << PC2) 													, // 7
		(1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6), // 8
		(1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | 			 (1 << PC5) | (1 << PC6)  // 9
	};

	uint8_t buffer[6];

	buffer[0] = 4;
	buffer[1] = 4;
	buffer[2] = 2;
	buffer[3] = 6;
	buffer[4] = 2;
	buffer[5] = 0;


	while(1)
	{
		// multipleksowanie 6 wyswietlaczy
		for(int i = 0; i < 6; i++)
		{
			SELECT_DISPLAY(i); // aktywacja wyswietlacza
			SET_DIGIT(buffer[i]); // ustawienie liczby na wyswietlaczu 
			_delay_ms(5); // 200Hz
		}
	}
	
	return 0;
}