#define FOREVER for( ; ; )

#include "clock.h"
#include "cio.h"
#include "io_controller.h"
#include "pio.h"
#include "pretty_io.h"
#include "switches.h"
#include "timer.h"
#include "trains.h"
#include "trains_controller.h"

#include <error.h>
#include <escape_codes.h>

void initialize_statics() {
    s_clock();
    s_cio();
    s_io_controller();
    s_pio();
    s_switches();
    s_timer();
    s_trains();
    s_trains_controller();
}

void initialize_uart() {

// COM1
// FIFO OFF
// Baud rate = 2400
// Start bits (if requested by computer) = 1
// Stop bits = 2
// Parity = None
// Word size = 8 bits
    pio_set_fifo(COM1, FIFO_OFF);
    pio_set_speed(COM1, 2400);
    pio_set_stop_bits(COM1, STOP_BITS_2);
    pio_set_wlen(COM1, WLEN_8BIT);

// COM2
// FIFO OFF
// Baud rate = 115200
    pio_set_fifo(COM2, FIFO_OFF);
    pio_set_speed(COM2, 115200);

}

void initialize_timer() {
    timer_enable();
}

void initialize_switches() {
    switches_set_curved();
}

void print_start_screen() {
    pretty_print_header();
}

int main(int argc, char* argv[]) {

    initialize_statics();
    initialize_uart();
    initialize_timer();

    print_start_screen();
    initialize_switches();

    while (io_controller_running()) {

        timer_update_ticks();
        timer_invoke();

    }

    pio_print_f(COM2, SCREEN_CLEAR);
    pio_flush(COM2);

    return SUCCESS;
}
