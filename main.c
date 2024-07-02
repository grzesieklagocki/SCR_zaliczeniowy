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

struct time
{
	uint8_t seconds,
		minutes,
		hours;
} current_time;


// deklaracje funkcji
void init_timer0(void);
void init_timer1(void);

void add_second(void);
void add_minute(void);
void add_hour(void);

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

	current_time.seconds = 57;
	current_time.minutes = 59;
	current_time.hours = 23;

	init_timer0();
	init_timer1();

	sei(); // zezwolenie globalne na przerwania

	while(1)
	{

	}
	
	return 0;
}

/****************************************************************/
// funkcja zwiekszajaca czas o 1s
/****************************************************************/
void add_second(void)
{
	if (current_time.seconds < 59)
		current_time.seconds++;
	else
	{
		current_time.seconds = 0;
		add_minute();
	}
}

/****************************************************************/
// funkcja zwiekszajaca czas o 1m
/****************************************************************/
void add_minute(void)
{
	if (current_time.minutes < 59)
		current_time.minutes++;
	else
	{
		current_time.minutes = 0;
		add_hour();
	}
}

/****************************************************************/
// funkcja zwiekszajaca czas o 1h
/****************************************************************/
void add_hour(void)
{
	if (current_time.hours < 23)
		current_time.hours++;
	else
		current_time.hours = 0;
}

/****************************************************************/
// inicjalizacja timera 0 i ustawienie na 180 Hz (do multipleksowania)
/****************************************************************/
void init_timer0(void)
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

/****************************************************************/
// inicjalizacja timera 1 i ustawienie na 1 Hz (do odmierzania sekund)
/****************************************************************/
void init_timer1(void)
{
	TCCR1B |=  (1 << WGM12); // tryb CTC
    TCCR1B |= (1 << CS12); // preskaler 256
	OCR1A = 31249; // (1 / czestotliwosc) / (1 / (F_CPU / preskaler))) - 1
    TIMSK |= (1 << OCIE1A); // zezwolenie na przerwanie timera 1 przy przepelnieniu
}

/****************************************************************/
// obsluga przerwania timer1 (do odmierzania sekund)
/****************************************************************/
ISR(TIMER1_COMPA_vect)
{
	buffer[0] = current_time.hours / 10;
	buffer[1] = current_time.hours % 10;
	buffer[2] = current_time.minutes / 10;
	buffer[3] = current_time.minutes % 10;
	buffer[4] = current_time.seconds / 10;
	buffer[5] = current_time.seconds % 10;

	add_second();
}

