#include "display.h"
#include "buttons.h"
#include "clock.h"
#include "settings.h"

#define BUTTON_STANDBY_TIMER_TOP 255
#define BUTTONS_ACTIVE_MAX_TIME 120

#define enable_timer_interrupt() TIMSK |= _BV(OCIE0A)
#define disable_timer_interrupt() TIMSK &= ~_BV(OCIE0A)

extern uint8_t interval[];
extern uint8_t duration[];

static uint8_t counter;
static bool blue_button_active;
static uint8_t blue_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
static bool red_button_active;
static uint8_t red_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;

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

bool are_buttons_active() {
    return blue_button_active || red_button_active;
}

static void reset_and_display_counter() {
    counter = 0;
    display_number(counter);
}

static void do_red_button_action() {
    switch (get_mode()) {
        case MENU_MODE:
        case SHOW_INTERVAL_MODE:
        case SETTINGS_MODE:
        case INTERVAL_SETTINGS_MODE:
            set_option(true);
            break;
        case INTERVAL_HOURS_SETTING_MODE:
            if (counter < 99) {
                display_number(++counter);
            }
            break;
        case INTERVAL_MINUTES_SETTING_MODE:
        case INTERVAL_SECONDS_SETTING_MODE:
            if (counter < 60) {
                display_number(++counter);
            }
            break;
    }
}

static void do_blue_button_action() {
    switch (get_mode()) {
        case OFF_MODE:
            set_mode(MENU_MODE);
            break;
        case MENU_MODE:
        case SHOW_INTERVAL_MODE:
        case SETTINGS_MODE:
        case INTERVAL_SETTINGS_MODE:
            set_option(false);
            break;
        case INTERVAL_HOURS_SETTING_MODE:
        case INTERVAL_MINUTES_SETTING_MODE:
        case INTERVAL_SECONDS_SETTING_MODE:
            if (counter > 0) {
                display_number(--counter);
            }
            break;
    }
}

static void do_both_button_action() {
    switch (get_mode()) {
        case MENU_MODE:
            switch (get_option()) {
                case MENU_SHOW_INTERVAL_OPTION:
                    set_mode(SHOW_INTERVAL_MODE);
                    break;
                case MENU_SHOW_DURATION_OPTION:
                    set_mode(SHOW_DURATION_MODE);
                    break;
                case MENU_SETTINGS_OPTION:
                    set_mode(SETTINGS_MODE);
                    break;
                default:
                    set_mode(OFF_MODE);
            }
            break;
        case SHOW_INTERVAL_MODE:
        case SHOW_DURATION_MODE:
            set_mode(MENU_MODE);
            break;
        case SETTINGS_MODE:
            switch (get_option()) {
                case SETTINGS_INTERVAL_SETTING_OPTION:
                    set_mode(INTERVAL_SETTINGS_MODE);
                    break;
                case SETTINGS_DURATION_SETTING_OPTION:
                    set_mode(DURATION_SETTINGS_MODE);
                    break;
                default:
                    set_mode(MENU_MODE);
            }
            break;
        case INTERVAL_SETTINGS_MODE:
            switch (get_option()) {
                case INTERVAL_SETTINGS_HOURS_OPTION:
                    set_mode(INTERVAL_HOURS_SETTING_MODE);
                    reset_and_display_counter();
                    break;
                case INTERVAL_SETTINGS_MINUTES_OPTION:
                    set_mode(INTERVAL_MINUTES_SETTING_MODE);
                    reset_and_display_counter();
                    break;
                case INTERVAL_SETTINGS_SECONDS_OPTION:
                    set_mode(INTERVAL_SECONDS_SETTING_MODE);
                    reset_and_display_counter();
                    break;
                default:
                    set_mode(SETTINGS_MODE);
            }
            break;
        case DURATION_SETTINGS_MODE:
            switch (get_option()) {
                case DURATION_SETTINGS_MINUTES_OPTION:
                    set_mode(DURATION_MINUTES_SETTING_MODE);
                    reset_and_display_counter();
                    break;
                case DURATION_SETTINGS_SECONDS_OPTION:
                    set_mode(DURATION_SECONDS_SETTING_MODE);
                    reset_and_display_counter();
                    break;
                default:
                    set_mode(SETTINGS_MODE);
            }
            break;
        case INTERVAL_HOURS_SETTING_MODE:
            set_interval_hours(counter);
            set_timer_top(interval[0], interval[1], interval[2]);
            set_mode(MENU_MODE);
            break;
        case INTERVAL_MINUTES_SETTING_MODE:
            set_interval_minutes(counter);
            set_timer_top(interval[0], interval[1], interval[2]);
            set_mode(MENU_MODE);
            break;
        case INTERVAL_SECONDS_SETTING_MODE:
            set_interval_seconds(counter);
            set_timer_top(interval[0], interval[1], interval[2]);
            set_mode(MENU_MODE);
            break;
        case DURATION_MINUTES_SETTING_MODE:
            set_duration_minutes(counter);
            set_mode(MENU_MODE);
            break;
        case DURATION_SECONDS_SETTING_MODE:
            set_duration_seconds(counter);
            set_mode(MENU_MODE);
            break;
    }
}

static void reset_buttons() {
    red_button_active = false;
    blue_button_active = false;
    blue_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
    red_button_wait_timer = BUTTON_STANDBY_TIMER_TOP;
}

static void handle_button_press(uint8_t *button_wait_timer, uint8_t *button_active) {
    wake_up();
    if (*button_wait_timer >= BUTTON_STANDBY_TIMER_TOP) {
        enable_timer_interrupt();
        *button_wait_timer = 0;
        *button_active = 1;
    }
}

static void handle_button_timer_interrupt(uint8_t *button_wait_timer, uint8_t *button_active,
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
            *button_active = 0;
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