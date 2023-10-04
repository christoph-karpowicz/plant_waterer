#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <avr/sleep.h>
#include "lib/init.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/i2c.h"
#include "lib/clock.h"
#include <util/delay.h>

#define TIMER_TOP 20

extern uint16_t ERROR;
extern uint16_t DOTS;
extern uint16_t EMPTY;

volatile bool display_greeting = true;
volatile bool timer_1_sec_mode;
volatile uint32_t timer_seconds;

void put_to_sleep() {
    if (!are_buttons_active()) {
        sleep();
    }
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

    put_to_sleep();
}

void init() {
    init_RTC_clock();
    init_buttons();
    init_clock_drivers();
    init_clock_interrupts();
    init_display();

    _delay_ms(500);
    sei();
}

int main(void) {
    init();
    set_sleep_mode(SLEEP_MODE_IDLE);
    while (1) {
        if (is_sleeping()) {
            sleep_enable();
            sleep_cpu();
            sleep_disable();
        }
    }
    return 0;
}