#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "lib/init.h"
#include "lib/display.h"
#include "lib/buttons.h"
#include "lib/i2c.h"
#include <util/delay.h>

extern uint16_t ERROR;
extern uint16_t DOTS;

ISR(INT1_vect) {
    display(ERROR);
}

ISR(PCINT0_vect) {
    display(DOTS);
}

ISR(PCINT1_vect) {
    display(DOTS);
}

void init() {
    init_display();
    init_RTC_clock();
    init_buttons();
    init_interrupts();

    _delay_ms(500);
    sei();
}

int main(void) {
    init();
    while (1) {
    }
    return 0;
}