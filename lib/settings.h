#ifndef SETTINGS_H
#define SETTINGS_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include "display.h"

#define MENU_MODE 0
#define SHOW_INTERVAL_MODE 1
#define SHOW_DURATION_MODE 2
#define SETTINGS_MODE 3
#define INTERVAL_SETTINGS_MODE 4
#define DURATION_SETTINGS_MODE 5
#define INTERVAL_HOURS_SETTING_MODE 6
#define INTERVAL_MINUTES_SETTING_MODE 7
#define INTERVAL_SECONDS_SETTING_MODE 8
#define DURATION_MINUTES_SETTING_MODE 9
#define DURATION_SECONDS_SETTING_MODE 10
#define OFF_MODE 11

#define DISPLAY_HOURS_OPTION 0
#define DISPLAY_MINUTES_OPTION 1
#define DISPLAY_SECONDS_OPTION 2

#define MENU_SHOW_INTERVAL_OPTION 0
#define MENU_SHOW_DURATION_OPTION 1
#define MENU_SETTINGS_OPTION 2

#define SETTINGS_INTERVAL_SETTING_OPTION 0
#define SETTINGS_DURATION_SETTING_OPTION 1

#define INTERVAL_SETTINGS_HOURS_OPTION 0
#define INTERVAL_SETTINGS_MINUTES_OPTION 1
#define INTERVAL_SETTINGS_SECONDS_OPTION 2

#define DURATION_SETTINGS_MINUTES_OPTION 0
#define DURATION_SETTINGS_SECONDS_OPTION 1

#define set_interval_hours(hours) interval[0] = hours
#define set_interval_minutes(minutes) interval[1] = minutes
#define set_interval_seconds(seconds) interval[2] = seconds

#define set_duration_minutes(minutes) duration[0] = minutes
#define set_duration_seconds(seconds) duration[1] = seconds

uint8_t get_mode();
void set_mode(uint8_t mode);
uint8_t get_option();
void set_option(int8_t next_option);

#endif