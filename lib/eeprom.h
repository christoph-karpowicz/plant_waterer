#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#define INTERVAL_HOURS_ADDRESS 0
#define INTERVAL_MINUTES_ADDRESS 1
#define INTERVAL_SECONDS_ADDRESS 2
#define DURATION_MINUTES_ADDRESS 3
#define DURATION_SECONDS_ADDRESS 4

void EEPROM_write(uint8_t ucAddress, uint8_t ucData);
uint8_t EEPROM_read(uint8_t ucAddress);

#endif