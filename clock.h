#ifndef CLOCK_H_
#define CLOCK_H_

// deklaracje funkcji
void init_clock(void); // inicjalizacja timera 1 i ustawienie na 1 Hz (do odmierzania sekund)

void clock_set_seconds(uint8_t seconds); // funkcja ustawiajaca sekundy
void clock_set_minutes(uint8_t minutes); // funkcja ustawiajaca minuty
void clock_set_hours(uint8_t hours); // funkcja ustawiajaca godzine

void add_second(void); // funkcja zwiekszajaca czas o 1s
void add_minute(void); // funkcja zwiekszajaca czas o 1m
void add_hour(void); // funkcja zwiekszajaca czas o 1h

#endif