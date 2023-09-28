#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
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
}

ISR(INT0_vect) {
    display(DOTS);
    timer_seconds = 0;
    timer_1_sec_mode = false;
    ten_seconds_clock_enable();
}

void init() {
    init_RTC_clock();
    init_buttons();
    init_clock_drivers();
    init_interrupts();
    init_display();

    _delay_ms(500);
    sei();
}

int main(void) {
    init();
    while (1) {
    }
    return 0;
}