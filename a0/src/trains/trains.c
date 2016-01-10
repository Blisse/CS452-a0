#include "trains.h"
#include "pio.h"
#include "cio.h"
#include "circular_buffer.h"

#include <error.h>

static circular_buffer_t train_command_buffer;
static unsigned int total_ticks;

void train_setup() {
    total_ticks = 0;
    circular_buffer_init(&train_command_buffer);
}

int train_set_speed(int train, int speed) {

    if (train < 1 || train > 80) {
        return E_FAIL;
    }

    if (speed < 0 || speed > 14) {
        return E_FAIL;
    }

    if (circular_buffer_full(&train_command_buffer)) {
        return E_FAIL;
    }

    circular_buffer_push(&train_command_buffer, speed);
    circular_buffer_push(&train_command_buffer, train);

    return SUCCESS;
}

int train_set_special_speed(int train, int speed) {

    if (train < 1 || train > 80) {
        return E_FAIL;
    }

    if (speed < 0 || speed > 14) {
        return E_FAIL;
    }

    if (circular_buffer_full(&train_command_buffer)) {
        return E_FAIL;
    }

    circular_buffer_push(&train_command_buffer, speed + 16);
    circular_buffer_push(&train_command_buffer, train);

    return SUCCESS;
}


int train_set_reverse(int train) {

    if (train < 1 || train > 80) {
        return E_FAIL;
    }

    if (!circular_buffer_full(&train_command_buffer)) {
        return E_FAIL;
    }

    circular_buffer_push(&train_command_buffer, 15);
    circular_buffer_push(&train_command_buffer, train);

    return SUCCESS;
}

int track_set_switch(int track_switch, char direction) {

    if (!circular_buffer_has_room(&train_command_buffer, 4)) {
        return E_FAIL;
    }

    if (direction == COMMAND_CHAR_SWITCH_STRAIGHT) {
        circular_buffer_push(&train_command_buffer, 33);
    } else {
        circular_buffer_push(&train_command_buffer, 34);
    }
    circular_buffer_push(&train_command_buffer, track_switch);

    circular_buffer_push(&train_command_buffer, 0xFF);
    circular_buffer_push(&train_command_buffer, 32);

    return SUCCESS;
}

int train_update() {
    char c;

    total_ticks++;

    if (total_ticks % 3 == 0) {

        if (SUCCEEDED(circular_buffer_get(&train_command_buffer, &c))) {

            if (c != 0xFF) {
                pio_put_char(COM1, c);
                pio_print_f(COM2, "Sent char %d \r\n", (int)c);
            }

            circular_buffer_pop(&train_command_buffer);
        }

        if (SUCCEEDED(circular_buffer_get(&train_command_buffer, &c))) {
            if (c != 0xFF) {
                pio_put_char(COM1, c);
                pio_print_f(COM2, "Sent char %d \r\n", (int)c);
            }

            circular_buffer_pop(&train_command_buffer);
        }

    }

    return SUCCESS;
}
