#include <avr/io.h>
#include "display.h"

/*
    10             13
   ___            ___
  |   |          |   |
8 |_9_| 11    12 |_2_|  14
  |   |          |   |
15|___| 6  .5  4 |___|  1  .0
    7              3 
*/
uint16_t EMPTY = 0b1111111111111111;
uint16_t HI = 0b0111010100100110;
uint16_t ERROR = 0b1001011000010000;
uint16_t DOTS = 0b0111101111111111;

void init_display() {
    DDRB |= _BV(SHIFT_REG_SCK_PIN);
    DDRB |= _BV(SHIFT_REG_RCK_PIN);
    DDRD |= _BV(SHIFT_REG_SERIAL_PIN);
    
    display(HI);
}

void display(uint16_t output) {
    SHIFT_REG_CK_PORT &= ~_BV(SHIFT_REG_RCK_PIN);
    uint8_t i;
    uint16_t mask = 0b1000000000000000;
    for (i = 0; i < OUTPUT_PINS; i++) {
        SHIFT_REG_CK_PORT &= ~_BV(SHIFT_REG_SCK_PIN);
        if (output & mask) {
            SHIFT_REG_SERIAL_PORT |= _BV(SHIFT_REG_SERIAL_PIN);
        } else {
            SHIFT_REG_SERIAL_PORT &= ~_BV(SHIFT_REG_SERIAL_PIN);
        }
        SHIFT_REG_CK_PORT |= _BV(SHIFT_REG_SCK_PIN);
        mask >>= 1;
    }
    SHIFT_REG_CK_PORT |= _BV(SHIFT_REG_RCK_PIN);
}