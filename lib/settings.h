#ifndef SETTINGS_H
#define SETTINGS_H

#define OFF_MODE 0
#define MENU_MODE 1

#define MENU_INTERVAL_SETTING_OPTION 0
#define MENU_EXIT_OPTION 1

void setting_mode_off();
uint8_t get_mode();
void set_mode(uint8_t mode);
uint8_t get_option();
void set_option(bool next);

#endif