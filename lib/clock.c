#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "display.h"

#define TIMER_TOP 60

volatile bool timer_1_sec_mode;
volatile bool sleep_mode_on;
volatile bool display_greeting = true;
volatile uint32_t timer_seconds;

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

void timer_1_sec_mode_on() {
    timer_1_sec_mode = true;
}

void timer_1_sec_mode_off() {
    timer_1_sec_mode = false;
}

void sleep() {
    if (!are_buttons_active()) {
        sleep_mode_on = true;
    }
}

void wake_up() {
    sleep_mode_on = false;
}

bool is_sleeping() {
    return sleep_mode_on;
}

// main clock interrupt
ISR(INT1_vect) {
    if (display_greeting) {
        display(EMPTY);
        display_greeting = false;
    }
    
    if (TIMER_TOP + 10 < timer_seconds) {
        display(EMPTY);
        timer_1_sec_mode = false;
        timer_seconds = 0;
        ten_seconds_clock_enable();
    } else if (TIMER_TOP < timer_seconds) {
        display(DOTS);
    } else if (TIMER_TOP - 10 <= timer_seconds) {
        timer_1_sec_mode = true;
        one_second_clock_enable();
        display_number(TIMER_TOP - timer_seconds);
    }

    if (!timer_1_sec_mode) {
        timer_seconds += 10;
    } else {
        timer_seconds++;
    }

    sleep();
}