#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>
#include <avr/pgmspace.h>

#define SHIFT_REG_SERIAL_PORT PORTD
#define SHIFT_REG_SERIAL_PIN PD6
#define SHIFT_REG_CK_PORT PORTB
#define SHIFT_REG_SCK_PIN PB0
#define SHIFT_REG_RCK_PIN PB1

/*
    10             13
   ___            ___
  |   |          |   |
8 |_9_| 11    12 |_2_|  14
  |   |          |   |
15|___| 6  .5  4 |___|  1  .0
    7              3 
*/
#define EMPTY 0b1111111111111111
#define DOTS 0b0111101111111111
                                        //  _
#define DISPLAY_EXIT 0b1010111001011000 // |_|

#define DISPLAY_SHOW_INTERVAL_OPTION 0b0011111111111101 // I.
#define DISPLAY_SHOW_DURATION_OPTION 0b0010110111101001 // D.
#define DISPLAY_SETTINGS_OPTION 0b1100010000010011 // SE
#define DISPLAY_SETTINGS_INTERVAL_OPTION 0b1011111111111101 // I
#define DISPLAY_SETTINGS_DURATION_OPTION 0b1010110111101001 // D
#define DISPLAY_SETTINGS_DAYS_OPTION 0b1000011111111101 // d
#define DISPLAY_SETTINGS_HOURS_OPTION 0b1001011111110101 // H
#define DISPLAY_SETTINGS_MINUTES_OPTION 0b1011111101000000 // M
#define DISPLAY_SETTINGS_SECONDS_OPTION 0b1000111111110011 // S

void display_number(const uint8_t number);
void display(const uint16_t output);

#endif