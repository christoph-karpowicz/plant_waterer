#include "eeprom.h"

void EEPROM_write(const uint8_t ucAddress, const uint8_t ucData) {
    while (EECR & _BV(EEPE));
    EECR = (0 << EEPM1) | (0 << EEPM0);
    EEAR = ucAddress;
    EEDR = ucData;
    EECR |= _BV(EEMPE);
    EECR |= _BV(EEPE);
}

const uint8_t EEPROM_read(const uint8_t ucAddress) {
    while (EECR & _BV(EEPE));
    EEAR = ucAddress;
    EECR |= _BV(EERE);
    return EEDR;
}