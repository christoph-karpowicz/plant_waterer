#include <avr/io.h>
#include "buttons.h"

void init_buttons() {
    BUTTON_DATA_DIRECTION_REG &= ~_BV(BLUE_BUTTON_PIN);
    BUTTON_DATA_DIRECTION_REG &= ~_BV(RED_BUTTON_PIN);
}