#include "cio.h"
#include "pio.h"
#include "trains.h"
#include "switches.h"
#include "io_controller.h"
#include "pretty_io.h"

#include <common.h>
#include <numbers.h>

#define COMMAND_BUFFER_SIZE     40

static char command_buffer[COMMAND_BUFFER_SIZE+1];
static int command_buffer_idx;

void s_cio() {
    cio_buffer_reset();
}

void cio_buffer_reset() {
    int i;

    command_buffer_idx = 0;
    for (i = 0; i < COMMAND_BUFFER_SIZE+1; i++) {
        command_buffer[i] = '\0';
    }
}

void cio_read() {
    char ch;

    if (SUCCEEDED(pio_get_char(COM2, &ch))) {

        cio_update_console(ch);

    }

}

int cio_update_console(char c) {

    if (!is_number(c)
        && !is_alpha(c)
        && (c != 8) // backspace
        && (c != ' ')
        && (c != '\r')
        && (c != '\n')) {
        return E_FAIL;
    }

    if (c == 8) {

        if (command_buffer_idx > 0) {
            command_buffer_idx -= 1;
        }

        command_buffer[command_buffer_idx] = '\0';

    } else if (c == COMMAND_CHAR_DONE_1) {

        cio_send_command(command_buffer, COMMAND_BUFFER_SIZE);

        cio_buffer_reset();

    } else if (command_buffer_idx < COMMAND_BUFFER_SIZE) {

        command_buffer[command_buffer_idx++] = c;

    }

    pretty_print_command(command_buffer, COMMAND_BUFFER_SIZE);

    return SUCCESS;
}

int cio_send_command(char* buffer, int buffer_size) {

    if (SUCCEEDED(cio_send_train_command(command_buffer, buffer_size))) {

    } else if (SUCCEEDED(cio_send_reverse_command(command_buffer, buffer_size))) {

    } else if (SUCCEEDED(cio_send_switch_command(command_buffer, buffer_size))) {

    } else if (SUCCEEDED(cio_send_halt_command(command_buffer, buffer_size))) {

    } else {

        return E_FAIL;
    }

    return SUCCESS;
}

int cio_send_train_command(char* buffer, int buffer_size) {
    int i = 0;
    int steps;
    int train;
    int speed;

    if (command_buffer[i++] != COMMAND_CHAR_TRAIN_1) {
        return E_FAIL;
    }

    if (command_buffer[i++] != COMMAND_CHAR_TRAIN_2) {
        return E_FAIL;
    }

    if (command_buffer[i++] != COMMAND_CHAR_SPACE) {
        return E_FAIL;
    }

    if (!is_number(command_buffer[i])) {
        return E_FAIL;
    }

    steps = 0;
    train = char_array_to_int(&command_buffer[i], &steps);
    i += steps;

    if (command_buffer[i++] != COMMAND_CHAR_SPACE) {
        return E_FAIL;
    }

    if (!is_number(command_buffer[i])) {
        return E_FAIL;
    }

    steps = 0;
    speed = char_array_to_int(&command_buffer[i], &steps);
    i += steps;

    train_set_speed(train, speed);

    return SUCCESS;
}

int cio_send_reverse_command(char* buffer, int buffer_size) {
    int i = 0;
    int steps;
    int train;

    if (command_buffer[i++] != COMMAND_CHAR_REVERSE_1) {
        return E_FAIL;
    }

    if (command_buffer[i++] != COMMAND_CHAR_REVERSE_2) {
        return E_FAIL;
    }

    if (command_buffer[i++] != COMMAND_CHAR_SPACE) {
        return E_FAIL;
    }

    if (!is_number(command_buffer[i])) {
        return E_FAIL;
    }

    steps = 0;
    train = char_array_to_int(&command_buffer[i], &steps);
    i += steps;

    train_set_reverse(train);

    return SUCCESS;
}

int cio_send_switch_command(char* buffer, int buffer_size) {
    int i = 0;
    int steps;
    int track_switch;
    char direction;

    if (command_buffer[i++] != COMMAND_CHAR_SWITCH_1) {
        return E_FAIL;
    }

    if (command_buffer[i++] != COMMAND_CHAR_SWITCH_2) {
        return E_FAIL;
    }

    if (command_buffer[i++] != COMMAND_CHAR_SPACE) {
        return E_FAIL;
    }

    if (!is_number(command_buffer[i])) {
        return E_FAIL;
    }

    steps = 0;
    track_switch = char_array_to_int(&command_buffer[i], &steps);
    i += steps;

    if (command_buffer[i++] != COMMAND_CHAR_SPACE) {
        return E_FAIL;
    }

    if (command_buffer[i] != COMMAND_CHAR_SWITCH_STRAIGHT
        && command_buffer[i] != COMMAND_CHAR_SWITCH_CURVED) {
        return E_FAIL;
    }

    direction = command_buffer[i++];

    switches_set_direction(track_switch, direction);

    return SUCCESS;
}

int cio_send_halt_command(char* buffer, int buffer_size) {
    int i = 0;

    if (command_buffer[i++] != COMMAND_CHAR_HALT_1) {
        return E_FAIL;
    }

    io_controller_quit();

    return SUCCESS;
}
