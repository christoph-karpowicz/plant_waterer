#ifndef I2C_H
#define I2C_H

#define I2C_DDR DDRD
#define I2C_PIN PIND
#define I2C_PORT PORTD

#define I2C_CLK PD4
#define I2C_DAT PD5

#define DS1307_ADDR 0x07
#define DS1307_SQUARE_WAVE_ENABLE 0x10
#define DS1307_W 0xD0

#define I2C_DATA_HIGH()\
I2C_DDR &= ~_BV(I2C_DAT);
#define I2C_DATA_LOW()\
I2C_DDR |= _BV(I2C_DAT);

#define I2C_CLOCK_HIGH()\
I2C_DDR &= ~_BV(I2C_CLK);
#define I2C_CLOCK_LOW()\
I2C_DDR |= _BV(I2C_CLK);

void init_RTC_clock();

#endif