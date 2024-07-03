#include <avr/io.h>
#include <avr/interrupt.h>

#include "led7seg.h"


struct time
{
	uint8_t seconds,
		minutes,
		hours;
} current_time;


// deklaracje funkcji
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

	init_led7seg();
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

