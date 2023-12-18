#include "pump.h"

static bool pump_on;

bool is_pump_on() {
    return pump_on;
}

void start_pump() {
    pump_on = true;
    PORTA |= _BV(PA0);
}

void stop_pump() {
    pump_on = false;
    PORTA &= ~_BV(PA0);
}