#include "trains.h"
#include "trains_controller.h"

#include <error.h>

#define COMMAND_GO      96
#define COMMAND_STOP    97
#define COMMAND_REVERSE 15


#define NUMBER_OF_TRAINS    81

static train_t trains[NUMBER_OF_TRAINS];

void s_trains() {
}

void trains_init() {
    int i;

    for (i = 0; i < NUMBER_OF_TRAINS; i++) {
        trains[i].number = i;
        trains[i].speed = 0;
    }
}

int train_go() {

    trains_controller_send_char(COMMAND_GO);

    return SUCCESS;
}

int train_stop() {

    trains_controller_send_char(COMMAND_STOP);

    return SUCCESS;
}

int train_set_speed(int train, int speed) {

    if (train < 1 || train > 80) {
        return E_FAIL;
    }

    if (speed < 0 || speed > 14) {
        return E_FAIL;
    }

    trains[train].speed = speed;

    trains_controller_send_chars(speed, train);

    return SUCCESS;
}

int train_set_reverse(int train) {
    int i;

    if (train < 1 || train > 80) {
        return E_FAIL;
    }

    trains_controller_send_chars(0, train);

    for (i = 0; i < 5; i += 1) {
        trains_controller_send_pause();
    }

    trains_controller_send_chars(COMMAND_REVERSE, train);

    for (i = 0; i < 5; i += 1) {
        trains_controller_send_pause();
    }

    trains_controller_send_chars(trains[train].speed, train);

    return SUCCESS;
}
