#include "timer.h"

#include <flags.h>
#include <ts7200.h>
#include <error.h>

#define MODE_FREE_RUNNING   0
#define MODE_PERIODIC       1

#define CLKSEL_2KHZ    0
#define CLKSEL_508KHZ  1

#define TIMER3_ADDRESS_VALUE    ADDRESS_OF_TIMER(TIMER3, VAL_OFFSET);
#define TIMER3_ADDRESS_LOAD     ADDRESS_OF_TIMER(TIMER3, LDR_OFFSET);
#define TIMER3_ADDRESS_CONTROL  ADDRESS_OF_TIMER(TIMER3, CRTL_OFFSET);

static unsigned int total_ticks;
static unsigned int last_tick;

void timer_setup() {
    total_ticks = 0;
    last_tick = 0;
}

int timer_enable() {
    int* control_addr = TIMER3_ADDRESS_CONTROL;
    int* load = TIMER3_ADDRESS_LOAD;
    int control = *control_addr;

    if (are_all_set(control, ENABLE_MASK)) {
        // timer is already enabled, don't do anything
        return E_NOCHANGE;
    }

    // enable the timer
    set_flags(&control, ENABLE_MASK);

    // set free running mode
    clear_flags(&control, MODE_MASK);

    // set 2kHz mode
    clear_flags(&control, CLKSEL_MASK);

    // roll-back from max int
    *load = 0xFFFFFFFF;

    // initialize the last tick from max int
    last_tick = 0xFFFFFFFF;

    // write to start timer
    *control_addr = control;

    return SUCCESS;
}

int timer_update_ticks() {
    int current_tick = *TIMER3_ADDRESS_VALUE;

    if (last_tick < current_tick) {
        total_ticks += last_tick;
        total_ticks += (0xFFFFFFFF - current_tick);
    } else {
        total_ticks += (last_tick - current_tick);
    }

    last_tick = current_tick;

    return SUCCESS;
}

int timer_get_ticks() {
    return total_ticks;
}
