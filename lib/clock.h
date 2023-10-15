#ifndef CLOCK_H
#define CLOCK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

void init_clock_interrupts();
void init_clock_drivers();
void wake_up();
bool is_sleeping();
void set_timer_top(uint8_t interval_hours, uint8_t interval_minutes,
                   uint8_t interval_seconds);

#endif