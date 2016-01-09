#include <ts7200.h>
#include <error.h>
#include "clock.h"

// 2kHz ticks = 0.5ms per tick
static unsigned int total_ticks;
static unsigned int last_tick;
static unsigned int hundred_ms_elapsed;

void clock_setup() {
    total_ticks = 0;
    last_tick = 0;
    hundred_ms_elapsed = 0;
}

// load the starting value into the load register.
// start a timer counting by setting the
// enable bit in the control/status register -- probably the 32-bit one.
// read the count register -- fine for polling loop, not fine for your kernel
int timer_enable() {
    int* load_addr = ADDRESS_OF_TIMER(TIMER3, LDR_OFFSET);
    int* control_addr = ADDRESS_OF_TIMER(TIMER3, CRTL_OFFSET);

    int control = *control_addr;

    if (control & ENABLE_MASK) {
        // timer is already enabled, don't do anything
        return E_NOCHANGE;
    }

    // enable the timer
    control |= ENABLE_MASK;

    // set free running mode
    control &= ~MODE_MASK;

    // set 2kHz mode
    control &= ~CLKSEL_MASK;

    // roll-back from max int
    *load_addr = 0xFFFFFFFF;

    // initialize the last tick from max int
    last_tick = 0xFFFFFFFF;

    // write to start timer
    *control_addr = control;

    return SUCCESS;
}

int timer_update() {
    int* value_addr = ADDRESS_OF_TIMER(TIMER3, VAL_OFFSET);
    int current_tick = *value_addr;

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

int clock_update() {
    int new_hundred_ms_elapsed = total_ticks / 200;
    if (new_hundred_ms_elapsed == hundred_ms_elapsed) {
        return E_NOCHANGE;
    }

    hundred_ms_elapsed = total_ticks / 200;
    return SUCCESS;
}

int clock_get_hundred_ms() {
    return hundred_ms_elapsed;
}
