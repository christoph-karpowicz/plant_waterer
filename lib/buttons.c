#include <avr/io.h>
#include <avr/interrupt.h>
#include "buttons.h"
#include "display.h"

void init_buttons() {
    GIMSK |= _BV(INT0);
    MCUCR |= (_BV(ISC01) | _BV(ISC00));
}