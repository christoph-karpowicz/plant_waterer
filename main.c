#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lib/display.h"
#include "lib/buttons.h"

void init() {
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