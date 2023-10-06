#ifndef CLOCK_H
#define CLOCK_H

void init_clock_interrupts();
void init_clock_drivers();
void timer_1_sec_mode_on();
void timer_1_sec_mode_off();
void sleep();
void wake_up();
bool is_sleeping();

#endif