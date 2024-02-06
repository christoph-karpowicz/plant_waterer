#include "clock.h"
#include "display.h"
#include "eeprom.h"

#define MIN_DURATION 10
#define TIMER_COUNTDOWN_TOP 10

#define MINUTES_TO_SECONDS(m)\
(m * 60)
#define HOURS_TO_SECONDS(h)\
(h * 60 * 60)
#define DAYS_TO_SECONDS(d)\
(d * 24 * 60 * 60)

volatile bool sleep_mode_on;

static volatile uint16_t duration = MIN_DURATION;
static volatile uint32_t timer;

static void sleep() {
    if (!are_buttons_active()) {
        sleep_mode_on = true;
    }
}

static void set_timer() {
    uint32_t timer_top = DAYS_TO_SECONDS(EEPROM_read(INTERVAL_DAYS_ADDRESS))
        + HOURS_TO_SECONDS(EEPROM_read(INTERVAL_HOURS_ADDRESS))
        + MINUTES_TO_SECONDS(EEPROM_read(INTERVAL_MINUTES_ADDRESS))
        - TIMER_COUNTDOWN_TOP;
    if (timer_top < duration + TIMER_COUNTDOWN_TOP) {
        timer_top = duration;
    }
    timer = timer_top;
}

static set_duration() {
    uint16_t duration_top = (MINUTES_TO_SECONDS(EEPROM_read(DURATION_MINUTES_ADDRESS))) 
        + EEPROM_read(DURATION_SECONDS_ADDRESS);
    if (duration_top < MIN_DURATION) {
        duration_top = MIN_DURATION;
    }
    duration = duration_top;
}

void set_duration_and_timer_top() {
    set_duration();
    set_timer();
}

// main clock interrupt
ISR(INT1_vect) {
    --timer;

    if (!(PINA & _BV(PA0))) { // is pump off
        if (timer > TIMER_COUNTDOWN_TOP) {
            goto end;
        }
        
        if (timer == 0) {
            set_timer();
            PORTA |= _BV(PA0); // start pump
            display(DOTS);
        } else {
            display_number(timer);
        }
    } else {
        --duration;
        if (duration == 0) {
            PORTA &= ~_BV(PA0); // stop pump
            set_duration();
            display(EMPTY);
        }
    }

end:
    sleep();
}