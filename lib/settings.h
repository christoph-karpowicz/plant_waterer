#ifndef SETTINGS_H
#define SETTINGS_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "display.h"

#define MENU_MODE 0
#define SHOW_INTERVAL_MODE 1
#define SETTINGS_MODE 2
#define INTERVAL_SETTINGS_MODE 3
#define INTERVAL_HOURS_SETTING_MODE 4
#define INTERVAL_MINUTES_SETTING_MODE 5
#define INTERVAL_SECONDS_SETTING_MODE 6
#define OFF_MODE 7

#define DISPLAY_INTERVAL_HOURS_OPTION 0
#define DISPLAY_INTERVAL_MINUTES_OPTION 1
#define DISPLAY_INTERVAL_SECONDS_OPTION 2

#define MENU_SHOW_INTERVAL_OPTION 0
#define MENU_SETTINGS_OPTION 1
#define MENU_EXIT_OPTION 2

#define SETTINGS_INTERVAL_SETTING_OPTION 0
#define SETTINGS_EXIT_OPTION 1

#define INTERVAL_SETTINGS_HOURS_OPTION 0
#define INTERVAL_SETTINGS_MINUTES_OPTION 1
#define INTERVAL_SETTINGS_SECONDS_OPTION 2
#define INTERVAL_SETTINGS_EXIT_OPTION 3

#define set_interval_hours(hours) interval[0] = hours
#define set_interval_minutes(minutes) interval[1] = minutes
#define set_interval_seconds(seconds) interval[2] = seconds

struct Options {
    uint8_t last;
    uint16_t opts[4];
};

uint8_t get_mode();
void set_mode(uint8_t mode);
uint8_t get_option();
void set_option(uint8_t next);

#endif