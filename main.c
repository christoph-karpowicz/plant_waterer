#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT PORTD
#define LED_PIN PD6

void init_led() {
    DDRD |= _BV(LED_PIN);
}

int main(void) {
    init_led();
    while (1) {
        LED_PORT &= ~_BV(LED_PIN);
        _delay_ms(1000);
        LED_PORT |= _BV(LED_PIN);
        _delay_ms(1000);
    }
    return 0;
}