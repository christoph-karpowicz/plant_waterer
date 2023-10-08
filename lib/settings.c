#include <avr/io.h>
#include "display.h"
#include "settings.h"

uint8_t interval[] = {
    0, // hours
    0, // minutes
    0 // seconds
};
uint8_t setting_mode;
uint8_t current_mode = OFF_MODE;
uint8_t current_option;

struct Options opts[] = {
    // MENU_MODE
    {
        .last = 2,
        .opts = {
            DISPLAY_SHOW_INTERVAL_OPTION,
            DISPLAY_SETTINGS_OPTION,
            DISPLAY_EXIT
        }
    },
    // SHOW_INTERVAL_MODE
    {
        .last = 2,
        .opts = {
            DISPLAY_INTERVAL_HOURS_OPTION,
            DISPLAY_INTERVAL_MINUTES_OPTION,
            DISPLAY_INTERVAL_SECONDS_OPTION
        }
    },
    // SETTINGS_MODE
    {
        .last = 1,
        .opts = {
            DISPLAY_SETTINGS_INTERVAL_OPTION,
            DISPLAY_EXIT
        }
    },
    // INTERVAL_SETTINGS_MODE
    {
        .last = 3,
        .opts = {
            DISPLAY_SETTINGS_HOURS_OPTION,
            DISPLAY_SETTINGS_MINUTES_OPTION,
            DISPLAY_SETTINGS_SECONDS_OPTION,
            DISPLAY_EXIT
        }
    }
};

void setting_mode_off() {
    set_mode(OFF_MODE);
}

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
    } else {
        display(opts[current_mode].opts[current_option]);
    }
}

uint8_t get_option() {
    return current_option;
}

void set_option(uint8_t next) {
    if (next) {
        if (current_option < opts[current_mode].last) {
            current_option++;
        }
    } else {
        if (current_option > 0) {
            current_option--;
        }
    }

    if (current_mode != SHOW_INTERVAL_MODE) {
        display(opts[current_mode].opts[current_option]);
    } else {
        display_number(interval[current_option]);
    }
}

void set_interval_hours(uint8_t hours) {
    interval[0] = hours;
}

void set_interval_minutes(uint8_t minutes) {
    interval[1] = minutes;
}

void set_interval_seconds(uint8_t seconds) {
    interval[2] = seconds;
}