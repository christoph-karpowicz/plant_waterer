#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"

uint8_t timer_1_sec_mode;
uint8_t sleep_mode_on;
uint8_t display_greeting = 1;
uint32_t timer_top = 60;
volatile uint32_t timer_seconds;

static void one_second_clock_enable() {
    PORTB |= (_BV(PB2) | _BV(PB3));
}

static void ten_seconds_clock_enable() {
    PORTB &= ~(_BV(PB2) | _BV(PB3));
}

static void timer_1_sec_mode_on() {
    timer_1_sec_mode = 1;
}

static void timer_1_sec_mode_off() {
    timer_1_sec_mode = 0;
}

static void sleep() {
    if (!are_buttons_active()) {
        sleep_mode_on = 1;
    }
}

void init_clock_drivers() {
    DDRB |= (_BV(PB2) | _BV(PB3));
}

void init_clock_interrupts() {
    // RTC clock timer rising edge interrupt
    GIMSK |= _BV(INT1);
    MCUCR |= (_BV(ISC11) | _BV(ISC10));
}

void wake_up() {
    sleep_mode_on = 0;
}

uint8_t is_sleeping() {
    return sleep_mode_on;
}

void set_timer_top(uint8_t interval_hours, uint8_t interval_minutes,
                   uint8_t interval_seconds) {
    timer_top = 0;
    timer_top += interval_hours * 60 * 60;
    timer_top += interval_minutes * 60;
    timer_top += interval_seconds;
}

// main clock interrupt
ISR(INT1_vect) {
    if (display_greeting) {
        display(EMPTY);
        display_greeting = 0;
    }
    
    if (timer_top + 10 < timer_seconds) {
        display(EMPTY);
        timer_1_sec_mode = 0;
        timer_seconds = 0;
        ten_seconds_clock_enable();
    } else if (timer_top < timer_seconds) {
        display(DOTS);
    } else if (timer_top - 10 <= timer_seconds) {
        timer_1_sec_mode = 1;
        one_second_clock_enable();
        display_number(timer_top - timer_seconds);
    }

    if (!timer_1_sec_mode) {
        timer_seconds += 10;
    } else {
        timer_seconds++;
    }

    sleep();
}