#include <avr/io.h>
#include <avr/interrupt.h>
#include "buttons.h"
#include "display.h"

void init_buttons() {
    RED_BUTTON_DDR &= ~_BV(RED_BUTTON_PIN);
    BLUE_BUTTON_DDR &= ~_BV(BLUE_BUTTON_PIN);

    PCMSK1 |= _BV(PCINT8);
    PCMSK |= _BV(PCINT2);
    GIMSK |= (_BV(PCIE) | _BV(3));
}