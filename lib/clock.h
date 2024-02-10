#ifndef CLOCK_H
#define CLOCK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define MINUTES_TO_SECONDS(m)\
(m * 60)
#define HOURS_TO_SECONDS(h)\
(h * 60 * 60)
#define DAYS_TO_SECONDS(d)\
(d * 24 * 60 * 60)

#define HOURS_TO_MINUTES(h)\
(h * 60)
#define DAYS_TO_MINUTES(d)\
(d * 24 * 60)

#define SECONDS_TO_MINUTES(s)\
((s) / 60)

#define wake_up() sleep_mode_on = false

uint16_t get_timer();
void set_duration_and_timer_top();

#endif