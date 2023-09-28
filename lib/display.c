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
uint16_t LEFT_DIGITS[] = {
    0b1111110001001110,
    0b1111110111101111,
    0b1111111010001110,
    0b1111110010001111,
    0b1111110100101111,
    0b1111110000011111,
    0b1111110000011110,
    0b1111110111001111,
    0b1111110000001110,
    0b1111110000001111
};
uint16_t RIGHT_DIGITS[] = {
    0b1010011111110001,
    0b1011111111111101,
    0b1100011111111001,
    0b1000111111111001,
    0b1001111111110101,
    0b1000111111110011,
    0b1000011111110011,
    0b1011111111111001,
    0b1000011111110001,
    0b1000111111110001
};

void init_display() {
    DDRB |= _BV(SHIFT_REG_SCK_PIN);
    DDRB |= _BV(SHIFT_REG_RCK_PIN);
    DDRD |= _BV(SHIFT_REG_SERIAL_PIN);
    
    display(HI);
}

void display_number(uint8_t number) {
    if (number > 99) {
        display(ERROR);
        return;
    }
    
    uint8_t left_segment = number / 10;
    uint8_t right_segment = number % 10;
    display(LEFT_DIGITS[left_segment] & RIGHT_DIGITS[right_segment]);
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