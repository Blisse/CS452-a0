#include "io_controller.h"
#include "cio.h"
#include "pio.h"

#include <escape_codes.h>
#include <common.h>

static int running;

void s_io_controller() {
    running = TRUE;
}

void io_controller_on_ms() {
    cio_read();
}

void io_controller_fetch() {
    pio_fetch(COM1);
    pio_fetch(COM2);
}

void io_controller_flush() {
    pio_flush(COM1);
    pio_flush(COM2);
}

int io_controller_running() {
    return running;
}

void io_controller_quit() {
    pio_print_f(COM2, SCREEN_CLEAR);
    io_controller_flush();
    running = FALSE;
}
