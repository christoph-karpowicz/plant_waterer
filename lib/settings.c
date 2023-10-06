#include <avr/io.h>
#include <stdbool.h>
#include "display.h"
#include "settings.h"

volatile uint32_t interval;
volatile bool setting_mode;
volatile uint8_t current_mode = OFF_MODE;
volatile uint8_t current_option = MENU_INTERVAL_SETTING_OPTION;

uint16_t options[] = {
    DISPLAY_INTERVAL_SETTING_OPTION,
    DISPLAY_EXIT
};
uint8_t last_option = 1;

void setting_mode_off() {
    set_mode(OFF_MODE);
}

uint8_t get_mode() {
    return current_mode;
}

void set_mode(uint8_t mode) {
    switch (mode)
    {
    case MENU_MODE:
        current_option = MENU_INTERVAL_SETTING_OPTION;
        display(options[current_option]);
        break;
    case OFF_MODE:
        display(EMPTY);
        break;
    default:
        break;
    }
    current_mode = mode;
}

uint8_t get_option() {
    return current_option;
}

void set_option(bool next) {
    if (next) {
        if (current_option < last_option) {
            current_option++;
        }
    } else {
        if (current_option > 0) {
            current_option--;
        }
    }
    display(options[current_option]);
}