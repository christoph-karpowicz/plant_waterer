#include "settings.h"

uint8_t interval[] = {
    0, // hours
    0, // minutes
    0 // seconds
};
uint8_t duration[] = {
    0, // minutes
    0 // seconds
};
static uint8_t setting_mode;
static uint8_t current_mode = OFF_MODE;
static uint8_t current_option;

uint16_t opts[6][3] PROGMEM = {
    // MENU_MODE
    {
        DISPLAY_SHOW_INTERVAL_OPTION,
        DISPLAY_SHOW_DURATION_OPTION,
        DISPLAY_SETTINGS_OPTION
    },
    // SHOW_INTERVAL_MODE
    {
        DISPLAY_HOURS_OPTION,
        DISPLAY_MINUTES_OPTION,
        DISPLAY_SECONDS_OPTION
    },
    // SHOW_DURATION_MODE
    {
        DISPLAY_MINUTES_OPTION,
        DISPLAY_SECONDS_OPTION,
        DISPLAY_EXIT
    },
    // SETTINGS_MODE
    {
        DISPLAY_SETTINGS_INTERVAL_OPTION,
        DISPLAY_SETTINGS_DURATION_OPTION,
        DISPLAY_EXIT
    },
    // INTERVAL_SETTINGS_MODE
    {
        DISPLAY_SETTINGS_HOURS_OPTION,
        DISPLAY_SETTINGS_MINUTES_OPTION,
        DISPLAY_SETTINGS_SECONDS_OPTION
    },
    // DURATION_SETTINGS_MODE
    {
        DISPLAY_SETTINGS_MINUTES_OPTION,
        DISPLAY_SETTINGS_SECONDS_OPTION,
        DISPLAY_EXIT
    }
};

uint8_t get_mode() {
    return current_mode;
}

void set_mode(uint8_t mode) {
    current_option = 0;
    current_mode = mode;

    if (current_mode == OFF_MODE) {
        display(EMPTY);
    } else if (current_mode == SHOW_INTERVAL_MODE) {
        display_number(interval[current_option]);
    } else if (current_mode == SHOW_DURATION_MODE) {
        display_number(duration[current_option]);
    } else {
        display(pgm_read_word(&opts[current_mode][current_option]));
    }
}

uint8_t get_option() {
    return current_option;
}

void set_option(bool next) {
    if (next) {
        if (current_option < 2) {
            current_option++;
        } else {
            display(DISPLAY_EXIT);
            return;
        }
    } else {
        if (current_option > 0) {
            current_option--;
        }
    }

    if (current_mode == SHOW_INTERVAL_MODE) {
        display_number(interval[current_option]);
    } else if (current_mode == SHOW_DURATION_MODE) {
        display_number(duration[current_option]);
    } else {
        display(pgm_read_word(&opts[current_mode][current_option]));
    }
}
