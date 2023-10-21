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
static uint8_t current_mode = OFF_MODE;
static int8_t current_option;

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
        0
    },
    // SETTINGS_MODE
    {
        DISPLAY_SETTINGS_INTERVAL_OPTION,
        DISPLAY_SETTINGS_DURATION_OPTION,
        0
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
        0
    }
};

const uint8_t get_mode() {
    return current_mode;
}

void set_mode(const uint8_t mode) {
    current_mode = mode;

    if (current_mode == OFF_MODE) {
        display(EMPTY);
    } else {
        set_option(0);
    }
}

const uint8_t get_option() {
    return current_option;
}

void set_option(const int8_t next_option) {
    if (next_option > 3 || next_option < 0) {
        return;
    }

    current_option = next_option;
    const uint16_t opt = pgm_read_word(&opts[current_mode][current_option]);

    if (current_option > 2 || opt == 0) {
        display(DISPLAY_EXIT);
        return;
    }

    if (current_mode == SHOW_INTERVAL_MODE) {
        display_number(interval[current_option]);
    } else if (current_mode == SHOW_DURATION_MODE) {
        display_number(duration[current_option]);
    } else {
        display(opt);
    }
}
