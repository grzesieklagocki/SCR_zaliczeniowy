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
// inicjalizacja timera 1 i ustawienie na 1 Hz (do odmierzania sekund)
/****************************************************************/
void init_clock(void)
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