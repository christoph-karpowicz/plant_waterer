#ifndef CLOCK_H
#define CLOCK_H

void init_clock_interrupts();
void init_clock_drivers();
void sleep();
void wake_up();
bool is_sleeping();

#endif