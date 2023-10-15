#include <avr/sleep.h>
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/i2c.h"
#include "lib/clock.h"

void init() {
    init_RTC_clock();
    init_buttons();
    init_clock_drivers();
    init_clock_interrupts();
    init_display();

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