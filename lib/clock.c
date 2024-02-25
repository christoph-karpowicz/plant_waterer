#include "clock.h"
#include "display.h"
#include "eeprom.h"
#include "buttons.h"

#define MIN_DURATION 10
#define TIMER_COUNTDOWN_TOP 10

extern uint8_t button_active_flags;

volatile bool sleep_mode_on;
static volatile uint16_t timer_minutes;
static volatile int8_t timer_seconds = 60;

static volatile uint16_t duration = MIN_DURATION;

static void sleep() {
    if (are_buttons_inctive()) {
        sleep_mode_on = true;
    }
}

static void set_timer() {
    uint16_t timer_top = (uint16_t)DAYS_TO_MINUTES(EEPROM_read(INTERVAL_DAYS_ADDRESS))
        + (uint16_t)HOURS_TO_MINUTES(EEPROM_read(INTERVAL_HOURS_ADDRESS))
        + (uint16_t)EEPROM_read(INTERVAL_MINUTES_ADDRESS);
    if (timer_top < SECONDS_TO_MINUTES(duration + 60)) {
        timer_top = SECONDS_TO_MINUTES(duration + 60);
    }
    timer_minutes = timer_top;
}

static set_duration() {
    uint16_t duration_top = (uint16_t)MINUTES_TO_SECONDS(EEPROM_read(DURATION_MINUTES_ADDRESS)) 
        + (uint16_t)EEPROM_read(DURATION_SECONDS_ADDRESS);
    if (duration_top < MIN_DURATION) {
        duration_top = MIN_DURATION;
    }
    duration = duration_top;
}

uint16_t get_timer() {
    return timer_minutes;
}

void set_duration_and_timer_top() {
    set_duration();
    set_timer();
}

// main clock interrupt
ISR(INT1_vect) {
    if (--timer_seconds <= 0) {
        --timer_minutes;
        timer_seconds = 60;
    }

    if (!(PINA & _BV(PA0))) { // is pump off
        if (timer_minutes > 1) {
            goto end;
        }
        
        if (timer_minutes == 0) {
            PORTA |= _BV(PA0); // start pump
            set_timer();
            display(DOTS);
        } else if (timer_seconds <= TIMER_COUNTDOWN_TOP) {
            display_number(timer_seconds);
        }
    } else {
        if (--duration == 0) {
            PORTA &= ~_BV(PA0); // stop pump
            set_duration();
            display(EMPTY);
        }
    }

end:
    sleep();
}