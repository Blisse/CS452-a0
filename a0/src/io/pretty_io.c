#include "pretty_io.h"
#include "pio.h"

#include <escape_codes.h>

void pretty_move_cursor(int row, int column) {
    pio_print_f(COM2, "\033[%d;%dH", row, column);
}

void pretty_print_header() {

    pretty_move_cursor(1, 4);

    pio_print_f(COM2, SCREEN_CLEAR);
    pio_flush(COM2);

    pio_print_f(COM2, CURSOR_HIDE);
    pio_flush(COM2);

    pio_print_f(COM2, ENDL);
    pio_print_f(COM2, TEXT_MAGENTA "~~RTOS v0.42~~" TEXT_RESET ENDL);
    pio_print_f(COM2, "    " "v6lai" ENDL);
    pio_print_f(COM2, TEXT_MAGENTA "~~~~~~~~~~~~~~" TEXT_RESET ENDL);

    pretty_move_cursor(7, 2);

    pio_print_f(COM2, TEXT_CYAN "> " TEXT_RESET);

    pio_flush(COM2);

}

void pretty_print_clock(int hundred_ms) {

    pretty_move_cursor(5, 3);

    pio_print_f(COM2, TEXT_CYAN "CLK" TEXT_RESET);

    pretty_move_cursor(5, 7);
    pio_print_f(COM2, "%d", ((((hundred_ms / 10) / 60) % 60) % 10));

    pretty_move_cursor(5, 8);
    pio_print_f(COM2, ":");
    pio_print_f(COM2, "%d", (hundred_ms / 10) % 60);

    pretty_move_cursor(5, 11);
    pio_print_f(COM2, ":");
    pio_print_f(COM2, "%d", (hundred_ms % 10));

    pio_flush(COM2);

}

void pretty_print_switches(switch_t* switches, int size, int sw, char dir) {
    int i;
    int line = 9;

    for (i = 0; i < size; i+=1) {

        if (switches[i].number == sw) {

            pretty_move_cursor(line, 4);
            pio_print_f(COM2, TEXT_YELLOW "s%d" TEXT_RESET, switches[i].number);

            pretty_move_cursor(line, 9);
            pio_print_f(COM2, "%c", switches[i].position);

            pio_flush(COM2);

            break;
        }

        line += 1;

    }

}

void pretty_print_command(char* command, int size) {
    int i;

    pretty_move_cursor(7, 4);

    for (i = 0; i < size; i++) {
        pio_put_char(COM2, ' ');
    }

    pretty_move_cursor(7, 4);
    pio_print_f(COM2, "%s", command);

    pio_flush(COM2);

}




