#ifndef LED7SEG_H_
#define LED7SEG_H_

extern volatile uint8_t buffer[6]; // bufor cyfr dla wyswietlaczy

// inicjalizacja timera 0 i ustawienie na 180 Hz (do multipleksowania)
void init_led7seg(void);

#endif