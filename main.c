#include <avr/io.h>
#include <avr/interrupt.h>

#include "led7seg.h"
#include "clock.h"
#include "adc.h"

#define LIMIT(arg, val)	((arg < val) ? arg : val) // makro ograniczajace wartosc

/****************************************************************/
// funkcja glowna programu
/****************************************************************/
int main(void)
{
	// inicjalizacja
	init_led7seg(); // inicjalizacja wyswietlaczy
	init_clock(); // inicjalizacja zegarka
	init_adc(); // inicjalizacja ADC

	sei(); // zezwolenie globalne na przerwania

	while(1)
	{
		refresh_displays();

		if (clock_tick)
		{
			clock_tick = 0;

			if (clock_mode == RUN)
			{
				add_second();
			}
			else // if (mode == ADJ)
			{
				// do zaimplementowania miganie
			}
			
		}

		if (clock_mode == RUN) // ponizej kod dla ustawiania zegarka pomijany w trybie RUN
			continue;
		
		uint16_t adc = read_adc(); // odczyt adc
		
		switch (cursor_adj) // ustawienie zegarka w zaleznosci odq pozycji kursora
		{
			case 0:
				clock_set_hours(adc / 43);
				break;
			case 1:
				clock_set_minutes(LIMIT(adc, 1003) / 17); // ograniczenie wartosci adc do 1003 (59*17)
				break;
			default:
				clock_set_seconds(LIMIT(adc, 1003) / 17);
				break;
		}
	}
	
	return 0;
}