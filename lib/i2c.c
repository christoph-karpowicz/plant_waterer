#include <avr/io.h>
#include <stdbool.h>
#include "init.h"
#include <util/delay.h>
#include "i2c.h"

static void delay() {
    // DS1307 has a 100Khz clock rate
    // F_CPU is 1Mhz
    // Delay of 10 us gives 100Khz
    _delay_us(10);
}

static void I2C_ack() {
    I2C_CLOCK_HIGH();
    I2C_DATA_HIGH();
    delay();
    I2C_CLOCK_LOW();
}

// Send single bit on data
static void I2C_write_data(uint8_t data) {
    if (data > 0) {
        I2C_DATA_HIGH();
    } else {
        I2C_DATA_LOW();
    }

    I2C_CLOCK_HIGH();
    delay();
    I2C_CLOCK_LOW();
    delay();
}

/*
Send a byte of data
       ____    ____    ____
SDA __/    \__/    \__/    \__
        __      __      __
SCL ___/  \____/  \____/  \___
*/
static void I2C_write(uint8_t data) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        I2C_write_data(data & 0b10000000);
        data <<= 1;
    }
    I2C_ack();
}

static void I2C_init() {
    I2C_PORT &= ~(_BV(I2C_DAT) | _BV(I2C_CLK));

    I2C_CLOCK_HIGH();
    I2C_DATA_HIGH();

    delay();
}

/*
Send start condition
    _______
SDA        \_________________
    _____________
SCL              \___________
*/ 
static void I2C_start() {
    I2C_DDR &= ~(_BV(I2C_DAT) | _BV(I2C_CLK));
    delay();

    I2C_DATA_LOW();
    delay();

    I2C_CLOCK_LOW();
    delay();
}

/*
Send stop condition
                 ____________
SDA ____________/
            _________________
SCL _______/
*/ 
static void I2C_stop() {
    I2C_CLOCK_HIGH();
    delay();

    I2C_DATA_HIGH();
    delay();
}

static void I2C_send(uint8_t addr, uint8_t data) {
    I2C_start();
    I2C_write(DS1307_W);
    I2C_write(addr);
    I2C_write(data);
    I2C_stop();
}

void init_RTC_clock() {
    I2C_init();
    _delay_ms(1);
    // Enable SQW/OUT with frequency of 1Hz
    I2C_send(DS1307_ADDR, DS1307_SQUARE_WAVE_ENABLE);
    _delay_ms(50);
    I2C_send(0x00, 0);
}