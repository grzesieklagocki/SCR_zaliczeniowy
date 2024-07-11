#ifndef CLOCK_H_
#define CLOCK_H_

enum modes { RUN, ADJ }; // tryby pracy zegarka: RUN - odmierzanie czasu, ADJ - ust. czasu
extern volatile uint8_t clock_mode; // aktualny tryb pracy, domyślnie uruchomiony
extern volatile uint8_t clock_tick; // flaga ustawiana w przerwaniu do uzycia w petli glownej
extern volatile uint8_t cursor_adj; // aktualna pozycja kursora

// struktura do przechowywania aktualnego czasu
typedef struct
{
	uint8_t seconds,
		minutes,
		hours;
} time;
extern time current_time; // zmienna przechowujaca aktualny czas

// deklaracje funkcji
void init_clock(void); // inicjalizacja zegarka

void clock_set_seconds(uint8_t seconds); // funkcja ustawiajaca sekundy
void clock_set_minutes(uint8_t minutes); // funkcja ustawiajaca minuty
void clock_set_hours(uint8_t hours); // funkcja ustawiajaca godzine

void add_second(void); // funkcja zwiekszajaca czas o 1s
void add_minute(void); // funkcja zwiekszajaca czas o 1m
void add_hour(void); // funkcja zwiekszajaca czas o 1h

void refresh_displays(void); // odswieza dane w buforze wyswietlaczy

void clock_set_mode(enum modes mode); // ustawia tryb pracy zegara

#endif