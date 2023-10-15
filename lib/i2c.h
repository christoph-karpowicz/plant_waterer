#ifndef I2C_H
#define I2C_H

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

void init_RTC_clock();

#endif