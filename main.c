#include <avr/io.h>
#include <avr/interrupt.h>

#include "led7seg.h"
#include "clock.h"

void init_buttons(void);

/****************************************************************/
// funkcja glowna programu
/****************************************************************/
int main(void)
{
	// inicjalizacja
	init_led7seg(); // inicjalizacja wyswietlaczy
	init_clock(); // inicjalizacja zegarka
	init_buttons(); // inicjalizacja przyciskow

	clock_set_seconds(57); // ustawienie sekund
	clock_set_minutes(59); // ustawienie minut
	clock_set_hours(23); // ustawienie godziny

	sei(); // zezwolenie globalne na przerwania

	while(1)
	{
		// kod programu w przerwaniach
	}
	
	return 0;
}

/****************************************************************/
// inicjalizacja obslugi przyciskow na przerwaniach
/****************************************************************/
void init_buttons(void)
{
	PORTD |= (1 << PD2) | (1 << PD3); // wlaczenie rezystora pull-up dla wejsc z przyciskami
	MCUCR |= (1 << ISC01) | (1 << ISC11); // zbocze opadajace dla INT0 i INT1
	GICR |= (1 << INT0) | (1 << INT1); // wlaczenie przerwan dla INT0 i INT1
}

/****************************************************************/
// obsluga nacisniecia przycisku SELECT
/****************************************************************/
ISR(INT0_vect) 
{
	add_hour();
}

/****************************************************************/
// obsluga nacisniecia przycisku SET
/****************************************************************/
ISR(INT1_vect)
{
	add_minute();
}