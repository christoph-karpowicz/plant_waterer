#include <avr/io.h>
#include <avr/interrupt.h>
#include "buttons.h"
#include "display.h"

// blue button pressed
ISR(INT0_vect) {
    display_number(1);
}

// red button pressed
ISR(PCINT_B_vect) {
    display_number(2);
}

void init_buttons() {
    // blue button interrupt
    GIMSK |= _BV(INT0);
    MCUCR |= _BV(ISC01);

    // red button interrupt
    GIMSK |= _BV(PCIE);
    PCMSK |= _BV(PCINT4);
}