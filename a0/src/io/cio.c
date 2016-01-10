#include "cio.h"
#include "common.h"
#include "pio.h"
#include "trains.h"

#include <numbers.h>

#define COMMAND_BUFFER_SIZE     12

static int quit;
static char command_buffer[COMMAND_BUFFER_SIZE];
static int command_buffer_idx;

void cio_setup() {
    quit = FALSE;
    cio_buffer_reset();
}

void cio_buffer_reset() {
    int i;

    command_buffer_idx = 0;
    for (i = 0; i < COMMAND_BUFFER_SIZE; i++) {
        command_buffer[i] = '\0';
    }
}

void cio_update() {
    char ch;

    if (SUCCEEDED(pio_get_char(COM2, &ch))) {

        cio_update_console(ch);

        if (ch == COMMAND_CHAR_DONE_1) {

            pio_put_char(COM2, '\n');

            if (cio_send_command(command_buffer, COMMAND_BUFFER_SIZE)) {

            }

            cio_buffer_reset();


        } else if (command_buffer_idx < COMMAND_BUFFER_SIZE) {

            command_buffer[command_buffer_idx++] = ch;

        }

   }

}

int cio_update_console(char c) {

    if (!is_number(c)
        && !is_alpha(c)
        && (c != ' ')
        && (c != '\r')
        && (c != '\n')) {
        return E_FAIL;
    }

    pio_print_f(COM2, "%c", c);

    return SUCCESS;
}

int cio_send_command(char* buffer, int buffer_size) {

    if (SUCCEEDED(cio_send_train_command(command_buffer, buffer_size))) {
        pio_print_f(COM2, "Send Train\r\n");
    } else if (SUCCEEDED(cio_send_reverse_command(command_buffer, buffer_size))) {
        pio_print_f(COM2, "Send Reverse\r\n");
    } else if (SUCCEEDED(cio_send_switch_command(command_buffer, buffer_size))) {
        pio_print_f(COM2, "Send Switch\r\n");
    } else if (SUCCEEDED(cio_send_halt_command(command_buffer, buffer_size))) {
        pio_print_f(COM2, "Send Halt\r\n");
    } else {
        pio_print_f(COM2, "No command\r\n");
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

    pio_print_f(COM2, "train: %d\r\n", train);

    if (command_buffer[i++] != COMMAND_CHAR_SPACE) {
        return E_FAIL;
    }

    if (!is_number(command_buffer[i])) {
        return E_FAIL;
    }

    steps = 0;
    speed = char_array_to_int(&command_buffer[i], &steps);
    i += steps;

    pio_print_f(COM2, "speed: %d\r\n", speed);

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

    if (!is_number(command_buffer[i++])) {
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

    pio_print_f(COM2, "switch: %d\r\n", track_switch);

    if (command_buffer[i++] != COMMAND_CHAR_SPACE) {
        return E_FAIL;
    }

    if (command_buffer[i] != COMMAND_CHAR_SWITCH_STRAIGHT
        && command_buffer[i] != COMMAND_CHAR_SWITCH_CURVED) {
        return E_FAIL;
    }

    direction = command_buffer[i++];

    pio_print_f(COM2, "direction: %c\r\n", direction);

    track_set_switch(track_switch, direction);

    return SUCCESS;
}

int cio_send_halt_command(char* buffer, int buffer_size) {
    int i = 0;

    if (command_buffer[i++] != COMMAND_CHAR_HALT_1) {
        return E_FAIL;
    }

    quit = TRUE;

    return SUCCESS;
}


int cio_quit() {
    return quit;
}
