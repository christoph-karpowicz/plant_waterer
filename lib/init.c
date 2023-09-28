#include <avr/io.h>

void init_interrupts() {
    // RTC clock timer rising edge interrupt
    GIMSK |= _BV(INT1);
    MCUCR |= (_BV(ISC11) | _BV(ISC10));
}