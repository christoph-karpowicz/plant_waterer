#ifndef CLOCK_H
#define CLOCK_H

void init_clock_interrupts();
void init_clock_drivers();
void wake_up();
uint8_t is_sleeping();
void set_timer_top(uint8_t interval_hours, uint8_t interval_minutes,
                   uint8_t interval_seconds);

#endif