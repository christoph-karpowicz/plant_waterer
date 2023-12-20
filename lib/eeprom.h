#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#define INTERVAL_DAYS_ADDRESS 0
#define INTERVAL_HOURS_ADDRESS 1
#define INTERVAL_MINUTES_ADDRESS 2
#define DURATION_MINUTES_ADDRESS 3
#define DURATION_SECONDS_ADDRESS 4

void EEPROM_write(const uint8_t ucAddress, const uint8_t ucData);
const uint8_t EEPROM_read(const uint8_t ucAddress);

#endif