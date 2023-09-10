#ifndef DISPLAY_H
#define DISPLAY_H

#define SHIFT_REG_SERIAL_PORT PORTD
#define SHIFT_REG_SERIAL_PIN PD6
#define SHIFT_REG_CK_PORT PORTB
#define SHIFT_REG_SCK_PIN PB0
#define SHIFT_REG_RCK_PIN PB1
#define OUTPUT_PINS 16

void init_display();
void display(uint16_t output);

#endif