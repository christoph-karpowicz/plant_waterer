#ifndef CLOCK_H
#define CLOCK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define MIN_TIMER_TOP 30

void init_clock_interrupts();
void init_clock_drivers();
void wake_up();
bool is_sleeping();
void set_timer_top();

#endif