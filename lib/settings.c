#include "settings.h"
#include "display.h"
#include "eeprom.h"
#include "clock.h"

uint8_t current_mode = OFF_MODE;
uint8_t current_option;

const uint16_t opts[7][4] PROGMEM = {
    // MENU_MODE
    {
        DISPLAY_SHOW_INTERVAL_OPTION,
        DISPLAY_SHOW_DURATION_OPTION,
        DISPLAY_SHOW_TIME_LEFT_OPTION,
        DISPLAY_SETTINGS_OPTION
    },
    // SHOW_INTERVAL_MODE
    {
        DISPLAY_DAYS_OPTION,
        DISPLAY_HOURS_OPTION,
        DISPLAY_MINUTES_OPTION,
        0
    },
    // SHOW_DURATION_MODE
    {
        DISPLAY_MINUTES_OPTION,
        DISPLAY_SECONDS_OPTION,
        0,
        0
    },
    // SHOW_TIME_LEFT_MODE
    {
        DISPLAY_DAYS_OPTION,
        DISPLAY_HOURS_OPTION,
        DISPLAY_MINUTES_OPTION,
        0
    },
    // SETTINGS_MODE
    {
        DISPLAY_SETTINGS_INTERVAL_OPTION,
        DISPLAY_SETTINGS_DURATION_OPTION,
        0,
        0
    },
    // INTERVAL_SETTINGS_MODE
    {
        DISPLAY_SETTINGS_DAYS_OPTION,
        DISPLAY_SETTINGS_HOURS_OPTION,
        DISPLAY_SETTINGS_MINUTES_OPTION,
        0
    },
    // DURATION_SETTINGS_MODE
    {
        DISPLAY_SETTINGS_MINUTES_OPTION,
        DISPLAY_SETTINGS_SECONDS_OPTION,
        0,
        0
    }
};

void set_mode(const uint8_t mode) {
    current_mode = mode;

    if (current_mode == OFF_MODE) {
        display(EMPTY);
    } else {
        set_option(0);
    }
}

void set_option(const uint8_t next_option) {
    if (next_option > 4) {
        return;
    }

    current_option = next_option;
    const uint16_t opt = pgm_read_word(&opts[current_mode - MENU_MODE_OFFSET][current_option]);

    if (current_option > 3 || opt == 0) {
        display(DISPLAY_EXIT);
        return;
    }

    if (current_mode == SHOW_INTERVAL_MODE) {
        display_number(EEPROM_read(current_option));
    } else if (current_mode == SHOW_DURATION_MODE) {
        display_number(EEPROM_read(current_option + DURATION_EEPROM_OFFSET));
    } else if (current_mode == SHOW_TIME_LEFT_MODE) {
        if (current_option == 0) {
            display_number(get_timer() / (uint16_t)DAYS_TO_MINUTES(1));
        } else if (current_option == 1) {
            display_number((get_timer() % (uint16_t)DAYS_TO_MINUTES(1)) / (uint16_t)HOURS_TO_MINUTES(1));
        } else {
            display_number((get_timer() % (uint16_t)HOURS_TO_MINUTES(1)));
        }
    } else {
        display(opt);
    }
}
