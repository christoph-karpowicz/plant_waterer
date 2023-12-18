#ifndef PUMP_H
#define PUMP_H

#include <avr/io.h>
#include <stdbool.h>

bool is_pump_on();
void start_pump();
void stop_pump();

#endif