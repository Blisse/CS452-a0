#include "trains_controller.h"
#include "circular_buffer.h"
#include "trains.h"
#include "clock.h"
#include "switches.h"
#include "pio.h"

#include <ts7200.h>
#include <escape_codes.h>
#include <error.h>

#define COMMAND_SIZE_ONE    '1'
#define COMMAND_SIZE_TWO    '2'
#define COMMAND_PAUSE       'P'


static circular_buffer_t train_command_buffer;

void s_trains_controller() {

    circular_buffer_init(&train_command_buffer);

}

void trains_controller_on_50ms() {
    char c;

    if (SUCCEEDED(pio_get_char(COM1, &c))) {

        switches_handle_sensor(c);

    } else {

        switches_request_sensor();

    }

}

void trains_controller_on_250ms() {

    char c;
    char c_1;
    char c_2;

    if (SUCCEEDED(circular_buffer_get(&train_command_buffer, &c))) {
        circular_buffer_pop(&train_command_buffer);

        switch (c) {
            case COMMAND_SIZE_ONE: {
                if (SUCCEEDED(circular_buffer_get(&train_command_buffer, &c_1))) {
                    circular_buffer_pop(&train_command_buffer);

                    pio_put_char(COM1, c_1);
                }
            }
            break;
            case COMMAND_SIZE_TWO: {
                if (SUCCEEDED(circular_buffer_get(&train_command_buffer, &c_1))) {
                    circular_buffer_pop(&train_command_buffer);

                    if (SUCCEEDED(circular_buffer_get(&train_command_buffer, &c_2))) {
                        circular_buffer_pop(&train_command_buffer);

                        pio_put_char(COM1, c_1);
                        pio_put_char(COM1, c_2);

                    }
                }
            }
            break;
            case COMMAND_PAUSE: {

            }
            break;
        }

    }

}

int trains_controller_send_char(char c) {

    circular_buffer_push(&train_command_buffer, COMMAND_SIZE_ONE);
    circular_buffer_push(&train_command_buffer, c);

    return SUCCESS;
}

int trains_controller_send_chars(char c_1, char c_2) {

    circular_buffer_push(&train_command_buffer, COMMAND_SIZE_TWO);
    circular_buffer_push(&train_command_buffer, c_1);
    circular_buffer_push(&train_command_buffer, c_2);

    return SUCCESS;
}

int trains_controller_send_pause() {

    circular_buffer_push(&train_command_buffer, COMMAND_PAUSE);

    return SUCCESS;
}
