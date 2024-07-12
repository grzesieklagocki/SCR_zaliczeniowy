#ifndef LED7SEG_H_
#define LED7SEG_H_

extern volatile uint8_t buffer[6]; // bufor cyfr dla wyswietlaczy


void init_led7seg(void); // inicjalizacja wyswietlaczy

void led7seg_start(void); // wlaczenie multipleksowania
void led7seg_stop(void); // wylaczenie multipleksowania

#endif