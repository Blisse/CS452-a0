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

static unsigned int total_ticks_2khz;
static unsigned int total_ticks_1ms;
static unsigned int total_ticks_25ms;
static unsigned int actual_ticks_25ms;

static unsigned int last_tick_2khz;
static unsigned int last_tick_25ms;

static unsigned int elapsed_250ms;
static unsigned int elapsed_100ms;
static unsigned int elapsed_50ms;

void s_timer() {
    total_ticks_2khz = 0;
    total_ticks_1ms = 0;
    total_ticks_25ms = 0;
    actual_ticks_25ms = 0;

    last_tick_2khz = 0xFFFFFFFFul;
    last_tick_25ms = 0;

    elapsed_250ms = 0;
    elapsed_100ms = 0;
    elapsed_50ms = 0;
}

int timer_enable() {
    int* control_addr = TIMER3_ADDRESS_CONTROL;
    int* load_addr = TIMER3_ADDRESS_CONTROL;
    int control = *control_addr;

    if (are_all_set(control, ENABLE_MASK)) {
        return E_NOCHANGE;
    }

    // enable the timer
    // set free running mode
    // set 2kHz mode

    set_flags(&control, ENABLE_MASK);
    clear_flags(&control, MODE_MASK);
    clear_flags(&control, CLKSEL_MASK);

    *control_addr = control;

    *load_addr = 0xFFFFFFFFul;

    return SUCCESS;
}

int timer_update_ticks() {

    unsigned int current_tick_2khz = *TIMER3_ADDRESS_VALUE;

    if (last_tick_2khz < current_tick_2khz) {
        total_ticks_2khz += last_tick_2khz;
        total_ticks_2khz += (0xFFFFFFFFul - current_tick_2khz);
    } else {
        total_ticks_2khz += (last_tick_2khz - current_tick_2khz);
    }

    total_ticks_1ms = total_ticks_2khz / 2;
    total_ticks_25ms = total_ticks_1ms / 25;

    if (last_tick_25ms != total_ticks_25ms) {
        last_tick_25ms = total_ticks_25ms;

        actual_ticks_25ms++;

        if (actual_ticks_25ms % 2 == 0) {
            elapsed_50ms = 1;
        }

        if (actual_ticks_25ms % 4 == 0) {
            elapsed_100ms = 1;
        }

        if (actual_ticks_25ms % 10 == 0) {
            elapsed_250ms = 1;
        }

    }

    last_tick_2khz = current_tick_2khz;

    return SUCCESS;
}

void timer_invoke() {

    io_controller_on_ms();

    io_controller_fetch();
    io_controller_flush();

    if (elapsed_50ms) {
        elapsed_50ms = 0;
        timer_invoke_on_50ms();
    }

    io_controller_fetch();
    io_controller_flush();

    if (elapsed_100ms) {
        elapsed_100ms = 0;
        timer_invoke_on_100ms();
    }

    io_controller_fetch();
    io_controller_flush();

    if (elapsed_250ms) {
        elapsed_250ms = 0;
        timer_invoke_on_250ms();
    }

    io_controller_fetch();
    io_controller_flush();

}

void timer_invoke_on_50ms() {

    trains_controller_on_50ms();
}

void timer_invoke_on_100ms() {

    clock_on_100ms();
}

void timer_invoke_on_250ms() {

    trains_controller_on_250ms();
}

unsigned int timer_get_25_ms() {
    return actual_ticks_25ms;
}
