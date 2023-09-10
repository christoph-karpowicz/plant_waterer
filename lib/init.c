#include <avr/io.h>

void init_interrupts() {
    GIMSK |= _BV(INT1);
    MCUCR |= (_BV(ISC11) | _BV(ISC10));
}