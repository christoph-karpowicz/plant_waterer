#ifndef CLOCK_H
#define CLOCK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define wake_up() sleep_mode_on = false

void set_duration_and_timer_top();

#endif