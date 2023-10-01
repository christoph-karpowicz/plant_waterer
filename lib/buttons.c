#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "buttons.h"
#include "display.h"

#define BUTTON_STANDBY_TIMER_TOP 255

extern uint16_t DOTS;

volatile uint8_t counter1;
volatile uint8_t counter2;
volatile bool blue_button_active;
volatile uint8_t blue_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
volatile bool red_button_active;
volatile uint8_t red_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;

// blue button pressed
ISR(INT0_vect) {
    if (blue_button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        handle_button_press(&blue_button_wait_timer, &blue_button_active);
        if (blue_button_active && red_button_active) {
            display(DOTS);
        } else {
            display_number(++counter1);
        }
    }
}

// red button pressed
ISR(PCINT_B_vect) {
    if (red_button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        handle_button_press(&red_button_wait_timer, &red_button_active);
        if (blue_button_active && red_button_active) {
            display(DOTS);
        } else {
            display_number(++counter2);
        }
    }
}

ISR(TIMER0_COMPA_vect) {
    handle_button_timer_interrupt(&blue_button_wait_timer, &blue_button_active);
    handle_button_timer_interrupt(&red_button_wait_timer, &red_button_active);
}

void init_buttons() {
    // blue button interrupt
    GIMSK |= _BV(INT0);
    MCUCR |= _BV(ISC01);

    // red button interrupt
    GIMSK |= _BV(PCIE);
    PCMSK |= _BV(PCINT4);

    // Timer divider 64 value
    TCCR0B |= (_BV(CS00) | _BV(CS01));
    // Timer CTC mode
    TCCR0A |= _BV(WGM01);
    // Timer TOP value
    OCR0A = 10;
}

void enable_timer_interrupt() {
    TIMSK |= _BV(OCIE0A);
}

void disable_timer_interrupt() {
    TIMSK &= ~_BV(OCIE0A);
}

void handle_button_press(uint8_t *button_wait_timer, bool *button_active) {
    enable_timer_interrupt();
    *button_wait_timer = 0;
    *button_active = true;
}

void handle_button_timer_interrupt(uint8_t *button_wait_timer, bool *button_active) {
    if (!(*button_active)) {
        return;
    }
    (*button_wait_timer)++;
    if (*button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        *button_active = false;
        if (!blue_button_active && !red_button_active) {
            disable_timer_interrupt();
        }
    }
}