#include <avr/io.h>
#include <avr/interrupt.h>


#define SELECT_DISPLAY(x)  PORTB = (1 << x);
#define SET_DIGIT(num)     PORTC = ((num < 0 || num > 9) ? 0 : segments[num]);
#define CURSOR_NEXT 	   cursor = (cursor + 1) % 6;


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


/****************************************************************/
// inicjalizacja timera 0 i ustawienie na 180 Hz (do multipleksowania)
/****************************************************************/
void init_led7seg(void)
{
    TCCR0 |= (1 << WGM01); // tryb CTC
    TCCR0 |= (1 << CS02) | (1 << CS00); // preskaler 1024
	OCR0 = 171; // (1 / czestotliwosc) / (1 / (F_CPU / preskaler))) - 1
    TIMSK |= (1 << OCIE0); // zezwolenie na przerwanie timera 0
}

/****************************************************************/
// obsluga przerwania timer0 (do multipleksowania)
/****************************************************************/
ISR(TIMER0_COMP_vect)
{
	SELECT_DISPLAY(cursor);	  // aktywacja wyswietlacza
	SET_DIGIT(buffer[cursor]); // ustawienie liczby na wyswietlaczu
	CURSOR_NEXT; // przejscie do nastepnego wyswietlacza
}