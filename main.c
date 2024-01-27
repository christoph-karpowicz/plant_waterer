#include <avr/sleep.h>
#include <avr/wdt.h>
#include "lib/display.h"
#include "lib/i2c.h"
#include <util/delay.h>
#include "lib/clock.h"

void init() {
    // init display
    DDRB |= _BV(SHIFT_REG_SCK_PIN) | _BV(SHIFT_REG_RCK_PIN);
    DDRD |= _BV(SHIFT_REG_SERIAL_PIN);

    // disable watchdog
    wdt_disable();
    // disable analog comparator
    ACSR |= _BV(ACD);
    // enable pull-ups
    PORTA |= _BV(PA1);
    PORTB |= _BV(PB2) | _BV(PB3);
    PORTD |= _BV(PD0) | _BV(PD1);

    init_RTC_clock();

    // init buttons and clock
    // red button interrupt
    PCMSK |= _BV(PCINT4);

    MCUCR |= 
        // blue button interrupt
        _BV(ISC01)
        // RTC clock timer rising edge interrupt
        | _BV(ISC11) | _BV(ISC10);
    //       blue btn    red btn     RTC clock
    GIMSK |= _BV(INT0) | _BV(PCIE) | _BV(INT1);

    // Timer divider 64 value
    TCCR0B |= _BV(CS00) | _BV(CS01);
    // Timer CTC mode
    TCCR0A |= _BV(WGM01);
    // Timer TOP value
    OCR0A = 10;
    
    // init pump driver
    DDRA |= _BV(PA0);

    set_duration_and_timer_top();

    display(EMPTY);
    sei();
}

int main(void) {
    init();
    set_sleep_mode(SLEEP_MODE_IDLE);
    while (1) {
        if (is_sleeping()) {
            sleep_enable();
            sleep_cpu();
            sleep_disable();
        }
    }
    return 0;
}