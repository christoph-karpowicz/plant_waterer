#include <avr/io.h>
#include "buttons.h"

void init_buttons() {
    BUTTON_DDR &= ~_BV(BLUE_BUTTON_PIN);
    BUTTON_DDR &= ~_BV(RED_BUTTON_PIN);
}