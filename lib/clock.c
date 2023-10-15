#include "clock.h"
#include "display.h"

static bool timer_1_sec_mode;
static bool sleep_mode_on;
static bool display_greeting = 1;
static uint32_t timer_top = 60;
static uint32_t timer_seconds;

static void one_second_clock_enable() {
    PORTB |= (_BV(PB2) | _BV(PB3));
}

static void ten_seconds_clock_enable() {
    PORTB &= ~(_BV(PB2) | _BV(PB3));
}

static void sleep() {
    if (!are_buttons_active()) {
        sleep_mode_on = true;
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
    sleep_mode_on = false;
}

bool is_sleeping() {
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
        timer_1_sec_mode = false;
        timer_seconds = 0;
        ten_seconds_clock_enable();
    } else if (timer_top < timer_seconds) {
        display(DOTS);
    } else if (timer_top - 10 <= timer_seconds) {
        timer_1_sec_mode = true;
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