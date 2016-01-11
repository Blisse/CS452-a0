#include "timer.h"
#include "io_controller.h"
#include "trains_controller.h"
#include "clock.h"
#include "pio.h"

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
static unsigned int total_50ms_ticks;
static unsigned int last_tick;

static unsigned int ticks_50ms_last;
static unsigned int ticks_50ms;

static unsigned int elapsed_250ms;
static unsigned int elapsed_100ms;
static unsigned int elapsed_50ms;

void s_timer() {
    total_ticks = 0;
    total_50ms_ticks = 0;
    last_tick = 0xFFFFFFFF;

    ticks_50ms_last = 0;
    ticks_50ms = 0;

    elapsed_250ms = 0;
    elapsed_100ms = 0;
    elapsed_50ms = 0;
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

    ticks_50ms = total_ticks / 100;

    if (ticks_50ms != ticks_50ms_last) {
        total_50ms_ticks += 1;

        if (total_50ms_ticks % 2 == 0) {
            elapsed_100ms = 1;
        }

        if (total_50ms_ticks % 5 == 0) {
            elapsed_250ms = 1;
        }

        elapsed_50ms = 1;

        ticks_50ms_last = ticks_50ms;
    }

    last_tick = current_tick;

    return SUCCESS;
}

void timer_invoke() {

    io_controller_fetch();

    io_controller_on_ms();

    io_controller_flush();

    if (elapsed_50ms) {
        elapsed_50ms = 0;

        io_controller_fetch();

        trains_controller_on_50ms();
    }

    io_controller_flush();

    if (elapsed_100ms) {
        elapsed_100ms = 0;

        clock_on_100ms();
    }

    io_controller_flush();

    if (elapsed_250ms) {
        elapsed_250ms = 0;

        trains_controller_on_250ms();
    }

    io_controller_flush();

}

int timer_get_50_ms() {
    return total_50ms_ticks;
}
