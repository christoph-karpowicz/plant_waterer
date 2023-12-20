#include "clock.h"
#include "display.h"
#include "eeprom.h"
#include "pump.h"

#define MIN_TIMER_TOP 20
#define MIN_DURATION 10
#define TIMER_COUNTDOWN_TOP 10

#define MINUTES_TO_SECONDS(m)\
(m * 60)
#define HOURS_TO_SECONDS(h)\
(h * 60 * 60)
#define DAYS_TO_SECONDS(d)\
(d * 24 * 60 * 60)

static bool sleep_mode_on;
static uint32_t timer_top = MIN_TIMER_TOP;
static uint16_t duration = MIN_DURATION;
static uint32_t timer_seconds;

static void sleep() {
    if (!are_buttons_active()) {
        sleep_mode_on = true;
    }
}

static void reset_timer() {
    timer_seconds = 0;
}

static void set_timer_top() {
    timer_top = DAYS_TO_SECONDS(EEPROM_read(INTERVAL_DAYS_ADDRESS))
        + HOURS_TO_SECONDS(EEPROM_read(INTERVAL_HOURS_ADDRESS))
        + MINUTES_TO_SECONDS(EEPROM_read(INTERVAL_MINUTES_ADDRESS))
        - TIMER_COUNTDOWN_TOP;
    if (timer_top < duration + TIMER_COUNTDOWN_TOP) {
        timer_top = duration;
    }
    reset_timer();
}

void wake_up() {
    sleep_mode_on = false;
}

bool is_sleeping() {
    return sleep_mode_on;
}

void set_duration_and_timer_top() {
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

    if (!is_pump_on()) {
        if (timer_top > timer_seconds) {
            goto end;
        }
        
        const uint32_t actual_timer_top = timer_top + TIMER_COUNTDOWN_TOP;
        if (actual_timer_top < timer_seconds) {
            display(DOTS);
            reset_timer();
            start_pump();
        } else if (timer_top <= timer_seconds) {
            display_number(actual_timer_top - timer_seconds);
        }
    } else {
        if (timer_seconds > duration) {
            stop_pump();
            display(EMPTY);
        } else {
            if (timer_seconds % 2 == 0) {
                display(LEFT_DOT);
            } else {
                display(RIGHT_DOT);
            }
        }
    }

end:
    sleep();
}