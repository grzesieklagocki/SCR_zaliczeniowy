#include <avr/io.h>
#include <avr/interrupt.h>

#include "clock.h"
#include "led7seg.h"


// struktura do przechowywania aktualnego czasu
struct time
{
	uint8_t seconds,
		minutes,
		hours;
} current_time; // zmienna przechowujaca aktualny czas


/****************************************************************/
// inicjalizacja zegarka
/****************************************************************/
void init_clock(void)
{
	// inicjalizacja timera 1 i ustawienie na 1 Hz (do odmierzania sekund)
	TCCR1B |=  (1 << WGM12); // tryb CTC
    TCCR1B |= (1 << CS12); // preskaler 256
	OCR1A = 31249; // (1 / czestotliwosc) / (1 / (F_CPU / preskaler))) - 1
    TIMSK |= (1 << OCIE1A); // zezwolenie na przerwanie timera 1 przy przepelnieniu

	// inicjalizacja przerwan INT0 i INT1 dla przycisków
	PORTD |= (1 << PD2) | (1 << PD3); // wlaczenie rezystora pull-up dla wejsc z przyciskami
	MCUCR |= (1 << ISC01) | (1 << ISC11); // zbocze opadajace dla INT0 i INT1
	GICR |= (1 << INT0) | (1 << INT1); // wlaczenie przerwan dla INT0 i INT1
}

/****************************************************************/
// funkcja ustawiajaca sekundy
/****************************************************************/
void clock_set_seconds(uint8_t seconds)
{
    current_time.seconds = seconds;
}

/****************************************************************/
// funkcja ustawiajaca minuty
/****************************************************************/
void clock_set_minutes(uint8_t minutes)
{
    current_time.minutes = minutes;
}

/****************************************************************/
// funkcja ustawiajaca godzine
/****************************************************************/
void clock_set_hours(uint8_t hours)
{
    current_time.hours = hours;
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
// odswieza dane w buforze wyswietlaczy
/****************************************************************/
void refresh_displays(void)
{
	buffer[0] = current_time.hours / 10;
	buffer[1] = current_time.hours % 10;
	buffer[2] = current_time.minutes / 10;
	buffer[3] = current_time.minutes % 10;
	buffer[4] = current_time.seconds / 10;
	buffer[5] = current_time.seconds % 10;
}

/****************************************************************/
// obsluga przerwania timer1 (do odmierzania sekund)
/****************************************************************/
ISR(TIMER1_COMPA_vect)
{
	refresh_displays();
	add_second();
}

/****************************************************************/
// obsluga nacisniecia przycisku SELECT
/****************************************************************/
ISR(INT0_vect) 
{
	add_hour();
	refresh_displays();
}

/****************************************************************/
// obsluga nacisniecia przycisku SET
/****************************************************************/
ISR(INT1_vect)
{
	add_minute();
	refresh_displays();
}