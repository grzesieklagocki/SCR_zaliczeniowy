#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define SELECT_DISPLAY(x)  PORTB = (1 << x);
#define SET_DIGIT(num)     PORTC = ((num < 0 || num > 9) ? 0 : segments[num]);
#define CURSOR_NEXT 	   cursor = (cursor + 1) % 6


volatile uint8_t cursor = 0; // aktualnie wybrany wyswietlacz przy multipleksowaniu/ustawianiu
volatile uint8_t buffer[6]; // bufor cyfr dla wyswietlaczy

// tablica wzorcow dla wyswietlacza 7-segmentowego
volatile const uint8_t segments[10] = 
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


// deklaracje funkcji
void init_timer0(void);

/****************************************************************/
// funkcja glowna programu
/****************************************************************/
int main(void)
{
	// inicjalizacja

	// PB[0-5] jako wyjscia - do wyboru wyswietlacza
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
	
	// PC[0-6] jako wyjscia - sterowanie wyswietlaczem 7-segmentowym
	DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6);

	buffer[0] = -1;
	buffer[1] = 10;
	buffer[2] = 7;
	buffer[3] = 7;
	buffer[4] = 1;
	buffer[5] = 6;

	init_timer0();

	sei();

	while(1)
	{

	}
	
	return 0;
}

/****************************************************************/
// inicjalizacja timera 0 i ustawienie na 180 Hz (do multipleksowania)
/****************************************************************/
void init_timer0(void)
{
    TCCR0 |= (1 << WGM01); // tryb CTC
    TCCR0 |= (1 << CS02) | (1 << CS00); // preskaler 1024
	OCR0 = 20;
    TIMSK |= (1 << OCIE0); // zezwolenie na przerwanie timera 0
}

/****************************************************************/
// obsluga przerwania timer0
/****************************************************************/
ISR(TIMER0_COMP_vect)
{
	SELECT_DISPLAY(cursor);	  // aktywacja wyswietlacza
	SET_DIGIT(buffer[cursor]); // ustawienie liczby na wyswietlaczu
	CURSOR_NEXT;
}
