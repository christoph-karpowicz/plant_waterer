#include <avr/io.h>

void init_clock_drivers() {
    DDRB |= (_BV(PB2) | _BV(PB3));
}

void one_second_clock_enable() {
    PORTB |= (_BV(PB2) | _BV(PB3));
}

void ten_seconds_clock_enable() {
    PORTB &= ~(_BV(PB2) | _BV(PB3));
}