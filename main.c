#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "lib/init.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/i2c.h"
#include <util/delay.h>

void init() {
    init_display();
    init_RTC_clock();

    _delay_ms(500);
    sei();
}

int main(void) {
    init();
    while (1) {
    }
    return 0;
}