#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define LED_PORT PORTB
#define LED_PIN PB4

#define SHIFT_REG_SERIAL_PORT PORTD
#define SHIFT_REG_SERIAL_PIN PD6
#define SHIFT_REG_CK_PORT PORTB
#define SHIFT_REG_SCK_PIN PB0
#define SHIFT_REG_RCK_PIN PB1
#define DISPLAY_PINS 16

/*
    10             13
   ___            ___
  |   |          |   |
8 |_9_| 11    12 |_2_|  14
  |   |          |   |
15|___| 6  .5  4 |___|  1  .0
    7              3 
*/
bool display_pins[DISPLAY_PINS] = {1,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1};

void init_led() {
    DDRB |= _BV(LED_PIN);
}

void init_display() {
    DDRB |= _BV(SHIFT_REG_SCK_PIN);
    DDRB |= _BV(SHIFT_REG_RCK_PIN);
    DDRD |= _BV(SHIFT_REG_SERIAL_PIN);
    
    SHIFT_REG_CK_PORT &= ~_BV(SHIFT_REG_RCK_PIN);
    uint8_t i;
    for (i = 0; i < DISPLAY_PINS; i++) {
        SHIFT_REG_CK_PORT &= ~_BV(SHIFT_REG_SCK_PIN);
        if (display_pins[i]) {
            SHIFT_REG_SERIAL_PORT |= _BV(SHIFT_REG_SERIAL_PIN);
        } else {
            SHIFT_REG_SERIAL_PORT &= ~_BV(SHIFT_REG_SERIAL_PIN);
        }
        SHIFT_REG_CK_PORT |= _BV(SHIFT_REG_SCK_PIN);
    }
    SHIFT_REG_CK_PORT |= _BV(SHIFT_REG_RCK_PIN);
}

int main(void) {
    init_led();
    init_display();
    while (1) {
        LED_PORT &= ~_BV(LED_PIN);
        _delay_ms(1000);
        LED_PORT |= _BV(LED_PIN);
        _delay_ms(1000);
    }
    return 0;
}