#include "clock.h"

#include <ts7200.h>
#include <error.h>
#include <escape_codes.h>
#include "pio.h"
#include "cio.h"
#include "timer.h"
#include "trains.h"

static unsigned int total_ticks;

void clock_setup() {
    total_ticks = 0;
}

int clock_update_ticks() {

    timer_update_ticks();

    int new_total_ticks = timer_get_ticks() / 200;

    int hundred_ms_elapsed = (new_total_ticks != total_ticks);

    total_ticks = new_total_ticks;

    clock_tick(hundred_ms_elapsed);

    return SUCCESS;
}

int clock_get_hundred_ms() {
    return total_ticks;
}

int clock_tick(int hundred_ms_elapsed) {

    if (hundred_ms_elapsed) {

        clock_update();
        cio_update();
        train_update();

    }

    return SUCCESS;
}

void clock_update() {
    // pio_print_f(COM2, DELETE_LINE);
    // pio_print_f(COM2, "\rt:");
    // pio_print_f(COM2, "%u", (((clock_get_hundred_ms() / 10) / 60) % 60));
    // pio_print_f(COM2, ":");
    // pio_print_f(COM2, "%u", (clock_get_hundred_ms() / 10) % 60);
    // pio_print_f(COM2, ":");
    // pio_print_f(COM2, "%u", clock_get_hundred_ms() % 10);
}
