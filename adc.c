#include <avr/io.h>

#include "adc.h"


/****************************************************************/
// inicjalizacja ADC
/****************************************************************/
void init_adc(void)
{
    DDRA &= ~(1 << PA0); // PA0 jako wejscie
    ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // wlaczenie ADC, preskaler 128
    ADMUX |= (1 << REFS0); // napiecie odniesienia AVCC
}

/****************************************************************/
// pomiar ADC
/****************************************************************/
uint16_t read_adc(void)
{
    ADCSRA |= (1 << ADSC); // start konwersji   
    
    while(!(1 << ADIF)); //oczekiwanie na koniec konwersji

    return (ADCH << 8) | ADCL; // zwrocenie wyniku 10-bitowego z obu rejestrow
}