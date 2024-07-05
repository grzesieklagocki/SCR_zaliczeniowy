#include <avr/io.h>
#include <avr/interrupt.h>

#include "led7seg.h"
#include "clock.h"
#include "adc.h"

/****************************************************************/
// funkcja glowna programu
/****************************************************************/
int main(void)
{
	// inicjalizacja
	init_led7seg(); // inicjalizacja wyswietlaczy
	//init_clock(); // inicjalizacja zegarka
	init_adc(); // inicjalizacja ADC

	clock_set_seconds(57); // ustawienie sekund
	clock_set_minutes(59); // ustawienie minut
	clock_set_hours(23); // ustawienie godziny

	sei(); // zezwolenie globalne na przerwania

	while(1)
	{
		// kod programu w przerwaniach
		buffer[0] = read_adc() / 103;
	}
	
	return 0;
}