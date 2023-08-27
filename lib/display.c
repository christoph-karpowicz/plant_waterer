#include "display.h"
#include <avr/io.h>
#include <stdbool.h>

/*
    10             13
   ___            ___
  |   |          |   |
8 |_9_| 11    12 |_2_|  14
  |   |          |   |
15|___| 6  .5  4 |___|  1  .0
    7              3 
*/
bool output_pins_state[OUTPUT_PINS] = {1,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1};

void init_display() {
    DDRB |= _BV(SHIFT_REG_SCK_PIN);
    DDRB |= _BV(SHIFT_REG_RCK_PIN);
    DDRD |= _BV(SHIFT_REG_SERIAL_PIN);
    
    SHIFT_REG_CK_PORT &= ~_BV(SHIFT_REG_RCK_PIN);
    uint8_t i;
    for (i = 0; i < OUTPUT_PINS; i++) {
        SHIFT_REG_CK_PORT &= ~_BV(SHIFT_REG_SCK_PIN);
        if (output_pins_state[i]) {
            SHIFT_REG_SERIAL_PORT |= _BV(SHIFT_REG_SERIAL_PIN);
        } else {
            SHIFT_REG_SERIAL_PORT &= ~_BV(SHIFT_REG_SERIAL_PIN);
        }
        SHIFT_REG_CK_PORT |= _BV(SHIFT_REG_SCK_PIN);
    }
    SHIFT_REG_CK_PORT |= _BV(SHIFT_REG_RCK_PIN);
}