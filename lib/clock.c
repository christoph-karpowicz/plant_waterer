#include <avr/io.h>
#include <stdbool.h>

volatile bool sleep_mode_on;

void init_clock_drivers() {
    DDRB |= (_BV(PB2) | _BV(PB3));
}

void init_clock_interrupts() {
    // RTC clock timer rising edge interrupt
    GIMSK |= _BV(INT1);
    MCUCR |= (_BV(ISC11) | _BV(ISC10));
}

void one_second_clock_enable() {
    PORTB |= (_BV(PB2) | _BV(PB3));
}

void ten_seconds_clock_enable() {
    PORTB &= ~(_BV(PB2) | _BV(PB3));
}

void sleep() {
    sleep_mode_on = true;
}

void wake_up() {
    sleep_mode_on = false;
}

bool is_sleeping() {
    return sleep_mode_on;
}