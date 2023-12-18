#include "clock.h"
#include "display.h"
#include "eeprom.h"

#define MIN_TIMER_TOP 30
#define MIN_DURATION 10
#define TIMER_COUNTDOWN_TOP 10

#define MINUTES_TO_SECONDS(m)\
(m * 60)
#define HOURS_TO_SECONDS(h)\
(h * 60 * 60)
#define DAYS_TO_SECONDS(d)\
(d * 24 * 60 * 60)

#define START_PUMP()\
pump_on = true;\
PORTA |= _BV(PA0);
#define STOP_PUMP()\
pump_on = false;\
PORTA &= ~_BV(PA0);

static bool sleep_mode_on;
static bool pump_on;
static bool display_greeting = true;
static uint32_t timer_top = MIN_TIMER_TOP;
static uint16_t duration = MIN_DURATION;
static uint32_t timer_seconds;

static void sleep() {
    if (!are_buttons_active()) {
        sleep_mode_on = true;
    }
}

void reset_timer() {
    timer_seconds = 0;
}

void wake_up() {
    sleep_mode_on = false;
}

bool is_sleeping() {
    return sleep_mode_on;
}

void set_timer_top() {
    timer_top = DAYS_TO_SECONDS(EEPROM_read(INTERVAL_DAYS_ADDRESS))
        + HOURS_TO_SECONDS(EEPROM_read(INTERVAL_HOURS_ADDRESS))
        + MINUTES_TO_SECONDS(EEPROM_read(INTERVAL_MINUTES_ADDRESS));
    if (timer_top < duration + TIMER_COUNTDOWN_TOP) {
        timer_top = duration + TIMER_COUNTDOWN_TOP;
    }
}

void set_duration() {
    duration = (MINUTES_TO_SECONDS(EEPROM_read(DURATION_MINUTES_ADDRESS))) 
        + EEPROM_read(DURATION_SECONDS_ADDRESS);
    if (duration < MIN_DURATION) {
        duration = MIN_DURATION;
    }
    set_timer_top();
}

// main clock interrupt
ISR(INT1_vect) {
    timer_seconds++;
    
    if (!pump_on) {
        if (display_greeting) {
            display(EMPTY);
            display_greeting = 0;
        }
        
        if (timer_top < timer_seconds) {
            display(DOTS);
            reset_timer();
            START_PUMP();
        } else if (timer_top - TIMER_COUNTDOWN_TOP <= timer_seconds) {
            display_number(timer_top - timer_seconds);
        }
    } else {
        if (timer_seconds > duration) {
            STOP_PUMP();
            display(EMPTY);
        }
    }

    sleep();
}