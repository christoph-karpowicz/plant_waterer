#ifndef CLOCK_H
#define CLOCK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

void wake_up();
bool is_sleeping();
void reset_timer();
void set_timer_top();

#endif