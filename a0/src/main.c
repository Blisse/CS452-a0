#define FOREVER for( ; ; )

#include "clock.h"
#include "trains.h"
#include "timer.h"
#include "pio.h"
#include "cio.h"
#include "switches.h"

#include <error.h>
#include <escape_codes.h>

void setup() {
    clock_setup();
    cio_setup();
    switch_setup();
    timer_setup();
    train_setup();
    pio_setup();
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
    // pio_set_parity(COM1, PARITY_OFF);
    pio_set_wlen(COM1, WLEN_8BIT);

// COM1
// FIFO OFF
// Baud rate = 115200
    pio_set_fifo(COM2, FIFO_OFF);
    pio_set_speed(COM2, 115200);
}

void initialize_timer() {
    timer_enable();
}

void print_start_screen() {
    pio_print_f(COM2, "\r\n~~RTOS v0.6~~\r\n");
    pio_flush(COM2);

    pio_print_f(COM2, "Timer Enabled\r\n");
    pio_print_f(COM2, "First Tick: %u \r\n", timer_get_ticks());
    pio_flush(COM2);

    pio_print_f(COM2, "-------------\r\n");
    pio_print_f(COM2, "Tests Started\r\n");
    pio_print_f(COM2, "Test #1: %d\r\n", 1);
    pio_print_f(COM2, "Test #2: %d %d\r\n", 2, 2);
    pio_print_f(COM2, "Test #3: %d %u %u\r\n", 3, 3, 3);
    pio_print_f(COM2, "Test #4: %u %d %u %d\r\n", 4, 3, 2, 1);
    pio_print_f(COM2, "\033[32m" "Tests Completed\r\n" "\033[0m");
    pio_print_f(COM2, "---------------\r\n");
    pio_flush(COM2);
}

int main(int argc, char* argv[]) {

    setup();
    initialize_uart();
    initialize_timer();
    print_start_screen();

    FOREVER {

        pio_fetch(COM1);
        pio_fetch(COM2);

        clock_update_ticks();

        pio_flush(COM1);
        pio_flush(COM2);

        if (cio_quit()) {
            break;
        }
    }

    return SUCCESS;
}
