#include <avr/io.h>
#include <avr/interrupt.h>

#include "clock.h"
#include "led7seg.h"
#include "adc.h"

#define CURSOR_NEXT 	   cursor_adj = (cursor_adj + 1) % 3; // kolejna pozycja kursora
#define CURSOR_RESET	   cursor_adj = 2; // reset kursora


volatile uint8_t clock_mode = RUN; // aktualny tryb pracy, domyślnie uruchomiony
volatile uint8_t clock_tick; // flaga ustawiana w przerwaniu do uzycia w petli glownej
volatile uint8_t cursor_adj; // aktualna pozycja kursora

time current_time;


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

	clock_set_mode(RUN); // ustawienie trybu RUN (praca normalna)
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
// ustawia tryb pracy zegara
/****************************************************************/
void clock_set_mode(enum modes mode)
{
	clock_mode = mode;

	if (mode == RUN)
	{
		OCR1A = 31249; // ustawienie dla 1 Hz
		CURSOR_RESET;
	}
	else // if (mode == ADJ)
	{
		OCR1A = 3125;
		CURSOR_NEXT;
	}
}

/****************************************************************/
// obsluga przerwania timer1 (do odmierzania sekund)
/****************************************************************/
ISR(TIMER1_COMPA_vect)
{
	clock_tick = 1; // ustawienie flagi do uzycia w petli glownej
}

/****************************************************************/
// obsluga nacisniecia przycisku SELECT
/****************************************************************/
ISR(INT0_vect) 
{
	clock_set_mode(ADJ);
}

/****************************************************************/
// obsluga nacisniecia przycisku SET
/****************************************************************/
ISR(INT1_vect)
{
	clock_set_mode(RUN);
}