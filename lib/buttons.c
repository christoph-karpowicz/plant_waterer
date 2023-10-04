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

static void enable_timer_interrupt() {
    TIMSK |= _BV(OCIE0A);
}

static void disable_timer_interrupt() {
    TIMSK &= ~_BV(OCIE0A);
}

static void do_red_button_action() {
    display_number(++counter1);
}

static void do_blue_button_action() {
    display_number(++counter2);
}

static void do_both_button_action() {
    display(DOTS);
}

static void reset_buttons() {
    red_button_active = false;
    blue_button_active = false;
    blue_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
    red_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
}

static void handle_button_press(uint8_t *button_wait_timer, bool *button_active) {
    if (*button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        enable_timer_interrupt();
        *button_wait_timer = 0;
        *button_active = true;
    }
}

static void handle_button_timer_interrupt(uint8_t *button_wait_timer, bool *button_active,
                                          void (*action_func)()) {
    if (!(*button_active)) {
        return;
    }
    (*button_wait_timer)++;
    if (*button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        if (blue_button_active && red_button_active) {
            do_both_button_action();
            reset_buttons();
        } else {
            (*action_func)();
            *button_active = false;
        }
        if (!blue_button_active && !red_button_active) {
            disable_timer_interrupt();
        }
    }
}

// blue button pressed
ISR(INT0_vect) {
    handle_button_press(&blue_button_wait_timer, &blue_button_active);
}

// red button pressed
ISR(PCINT_B_vect) {
    handle_button_press(&red_button_wait_timer, &red_button_active);
}

// button timer interrupt
ISR(TIMER0_COMPA_vect) {
    handle_button_timer_interrupt(&blue_button_wait_timer, &blue_button_active, &do_blue_button_action);
    handle_button_timer_interrupt(&red_button_wait_timer, &red_button_active, &do_red_button_action);
}