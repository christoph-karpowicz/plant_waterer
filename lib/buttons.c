#include "buttons.h"
#include "display.h"
#include "clock.h"
#include "settings.h"
#include "eeprom.h"

#define BUTTON_STANDBY_TIMER_TOP 255
#define BUTTONS_ACTIVE_MAX_TIME 120
#define BLUE_BUTTON_ACTIVE_BIT 7
#define RED_BUTTON_ACTIVE_BIT 6

#define enable_timer_interrupt() TIMSK |= _BV(OCIE0A)
#define disable_timer_interrupt() TIMSK &= ~_BV(OCIE0A)

extern uint8_t current_mode;
extern uint8_t current_option;
extern bool sleep_mode_on;

// 0b10000000 - blue button active
// 0b01000000 - red button active
uint8_t button_active_flags = 0;

static uint8_t counter;
static uint8_t blue_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
static uint8_t red_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;

static void reset_and_display_counter() {
    counter = 0;
    display_number(counter);
}

static void do_red_button_action() {
    switch (current_mode) {
        case MENU_MODE:
        case SHOW_INTERVAL_MODE:
        case SHOW_DURATION_MODE:
        case SHOW_TIME_LEFT_MODE:
        case SETTINGS_MODE:
        case INTERVAL_SETTINGS_MODE:
        case DURATION_SETTINGS_MODE:
            set_option(current_option + 1);
            break;
        case INTERVAL_DAYS_SETTING_MODE:
            if (counter < 45) {
                display_number(++counter);
            }
            break;
        case INTERVAL_HOURS_SETTING_MODE:
            if (counter < 23) {
                display_number(++counter);
            }
            break;
        case INTERVAL_MINUTES_SETTING_MODE:
        case DURATION_MINUTES_SETTING_MODE:
        case DURATION_SECONDS_SETTING_MODE:
            if (counter < 59) {
                display_number(++counter);
            }
            break;
    }
}

static void do_blue_button_action() {
    switch (current_mode) {
        case OFF_MODE:
            set_mode(MENU_MODE);
            break;
        case MENU_MODE:
        case SHOW_INTERVAL_MODE:
        case SHOW_DURATION_MODE:
        case SHOW_TIME_LEFT_MODE:
        case SETTINGS_MODE:
        case INTERVAL_SETTINGS_MODE:
        case DURATION_SETTINGS_MODE:
            set_option(current_option - 1);
            break;
        case INTERVAL_DAYS_SETTING_MODE:
        case INTERVAL_HOURS_SETTING_MODE:
        case INTERVAL_MINUTES_SETTING_MODE:
        case DURATION_MINUTES_SETTING_MODE:
        case DURATION_SECONDS_SETTING_MODE:
            if (counter > 0) {
                display_number(--counter);
            }
            break;
    }
}

static void do_both_button_action() {
    const int8_t opt = current_option;
    switch (current_mode) {
        case MENU_MODE:
            if (opt == MENU_SHOW_INTERVAL_OPTION) {
                set_mode(SHOW_INTERVAL_MODE);
            } else if (opt == MENU_SHOW_DURATION_OPTION) {
                set_mode(SHOW_DURATION_MODE);
            } else if (opt == MENU_SHOW_TIME_LEFT_OPTION) {
                set_mode(SHOW_TIME_LEFT_MODE);
            } else if (opt == MENU_SETTINGS_OPTION) {
                set_mode(SETTINGS_MODE);
            } else {
                set_mode(OFF_MODE);
            }
            break;
        case SHOW_INTERVAL_MODE:
        case SHOW_DURATION_MODE:
        case SHOW_TIME_LEFT_MODE:
            set_mode(MENU_MODE);
            break;
        case SETTINGS_MODE:
            if (opt == SETTINGS_INTERVAL_SETTING_OPTION) {
                set_mode(INTERVAL_SETTINGS_MODE);
            } else if (opt == SETTINGS_DURATION_SETTING_OPTION) {
                set_mode(DURATION_SETTINGS_MODE);
            } else {
                set_mode(MENU_MODE);
            }
            break;
        case INTERVAL_SETTINGS_MODE:
            if (opt == INTERVAL_SETTINGS_DAYS_OPTION) {
                set_mode(INTERVAL_DAYS_SETTING_MODE);
                reset_and_display_counter();
            } else if (opt == INTERVAL_SETTINGS_HOURS_OPTION) {
                set_mode(INTERVAL_HOURS_SETTING_MODE);
                reset_and_display_counter();
            } else if (opt == INTERVAL_SETTINGS_MINUTES_OPTION) {
                set_mode(INTERVAL_MINUTES_SETTING_MODE);
                reset_and_display_counter();
            } else {
                set_mode(SETTINGS_MODE);
            }
            break;
        case DURATION_SETTINGS_MODE:
            if (opt == DURATION_SETTINGS_MINUTES_OPTION) {
                set_mode(DURATION_MINUTES_SETTING_MODE);
                reset_and_display_counter();
            } else if (opt == DURATION_SETTINGS_SECONDS_OPTION) {
                set_mode(DURATION_SECONDS_SETTING_MODE);
                reset_and_display_counter();
            } else {
                set_mode(SETTINGS_MODE);
            }
            break;
        case INTERVAL_DAYS_SETTING_MODE:
        case INTERVAL_HOURS_SETTING_MODE:
        case INTERVAL_MINUTES_SETTING_MODE:
        case DURATION_MINUTES_SETTING_MODE:
        case DURATION_SECONDS_SETTING_MODE:
            // EEPROM addresses are aligned with mode numbers
            EEPROM_write(current_mode, counter);
            set_duration_and_timer_top();
            set_mode(MENU_MODE);
            break;
    }
}

static void reset_buttons() {
    button_active_flags = 0;
    blue_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
    red_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
}

static void handle_button_press(uint8_t *button_wait_timer, uint8_t button_active) {
    if (PINA & _BV(PA0)) { // is pump on
        PORTA &= ~_BV(PA0); // stop pump
        set_mode(OFF_MODE);
        return;
    }
    wake_up();
    if (*button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        enable_timer_interrupt();
        *button_wait_timer = 0;
        button_active_flags |= _BV(button_active);
    }
}

static void handle_button_timer_interrupt(uint8_t *button_wait_timer, uint8_t button_active,
                                          void (*action_func)()) {
    if (!(button_active_flags & _BV(button_active))) {
        return;
    }
    (*button_wait_timer)++;
    if (*button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        if (are_both_buttons_active()) {
            do_both_button_action();
            reset_buttons();
        } else {
            (*action_func)();
            button_active_flags &= ~_BV(button_active);
        }
        if (are_buttons_inctive()) {
            disable_timer_interrupt();
        }
    }
}

// blue button pressed
ISR(INT0_vect) {
    handle_button_press(&blue_button_wait_timer, BLUE_BUTTON_ACTIVE_BIT);
}

// red button pressed
ISR(PCINT_B_vect) {
    handle_button_press(&red_button_wait_timer, RED_BUTTON_ACTIVE_BIT);
}

// button timer interrupt
ISR(TIMER0_COMPA_vect) {
    handle_button_timer_interrupt(&blue_button_wait_timer, BLUE_BUTTON_ACTIVE_BIT, &do_blue_button_action);
    handle_button_timer_interrupt(&red_button_wait_timer, RED_BUTTON_ACTIVE_BIT, &do_red_button_action);
}