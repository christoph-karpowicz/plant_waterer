#include "display.h"

#define OUTPUT_PINS 16

static const uint16_t LEFT_DIGITS[] PROGMEM = {
    0b1111110001001110,
    0b1111110111101111,
    0b1111111010001110,
    0b1111110010001111,
    0b1111110100101111,
    0b1111110000011111
};
static const uint16_t RIGHT_DIGITS[] PROGMEM = {
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

void display_number(const uint8_t number) {
    const uint8_t left_segment = number / 10;
    const uint8_t right_segment = number % 10;
    display(pgm_read_word(&LEFT_DIGITS[left_segment]) 
        & pgm_read_word(&RIGHT_DIGITS[right_segment]));
}

void display(const uint16_t output) {
    SHIFT_REG_CK_PORT &= ~_BV(SHIFT_REG_RCK_PIN);
    uint8_t i;
    uint16_t mask = 0b1000000000000000;
    for (i = OUTPUT_PINS; i > 0; i--) {
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