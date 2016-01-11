#include "switches.h"
#include "trains_controller.h"
#include "pretty_io.h"

#include <error.h>

#define COMMAND_SWITCH_OFF      32
#define COMMAND_SWITCH_STRAIGHT 33
#define COMMAND_SWITCH_CURVED   34

#define COMMAND_SENSOR_FETCH    133

#define NUMBER_OF_SWITCHES  22

static switch_t switches[NUMBER_OF_SWITCHES];

#define NUMBER_OF_SENSORS   5
#define SIZE_OF_SENSOR_DATA 2

static char sensor_data[NUMBER_OF_SENSORS * SIZE_OF_SENSOR_DATA];

static int reading;

void switches_reset() {
    int i;

    for (i = 0; i < NUMBER_OF_SWITCHES; i++) {
        switches[i].number = -1;
        switches[i].position = SWITCH_UNKNOWN;
    }
}

void switches_init() {
    switches_reset();

    switches[0].number = 1;
    switches[1].number = 2;
    switches[2].number = 3;
    switches[3].number = 4;
    switches[4].number = 5;
    switches[5].number = 6;
    switches[6].number = 7;
    switches[7].number = 8;
    switches[8].number = 9;
    switches[9].number = 10;
    switches[10].number = 11;
    switches[11].number = 12;
    switches[12].number = 13;
    switches[13].number = 14;
    switches[14].number = 15;
    switches[15].number = 16;
    switches[16].number = 17;
    switches[17].number = 18;
    switches[18].number = 153;
    switches[19].number = 154;
    switches[20].number = 155;
    switches[21].number = 156;
}

void sensors_init() {
    int i;

    for (i = 0; i < NUMBER_OF_SENSORS*SIZE_OF_SENSOR_DATA; i+=1) {
        sensor_data[i] = '0';
    }
}

void s_switches() {
    reading = -1;
    switches_init();
    sensors_init();
}

int switches_set_direction(int sw, char direction) {
    int i;

    if (direction != SWITCH_STRAIGHT && direction != SWITCH_CURVED) {
        return E_FAIL;
    }

    for (i = 0; i < NUMBER_OF_SWITCHES; i++) {
        if (switches[i].number == sw) {
            switches[i].position = direction;
        }
    }

    trains_controller_send_chars(direction == SWITCH_STRAIGHT ? COMMAND_SWITCH_STRAIGHT : COMMAND_SWITCH_CURVED, sw);
    trains_controller_send_char(COMMAND_SWITCH_OFF);
    trains_controller_send_pause();

    pretty_print_switches(switches, NUMBER_OF_SWITCHES, sw, direction);

    return SUCCESS;
}

int switches_set_curved() {
    int i;

    for (i = 0; i < NUMBER_OF_SWITCHES; i++) {
        switches_set_direction(switches[i].number, SWITCH_CURVED);
    }

    return SUCCESS;
}

char* switches_get_sensor() {
    return sensor_data;
}

int switches_request_sensor() {

    if (reading == -1) {

        trains_controller_send_char(COMMAND_SENSOR_FETCH);

        reading = 0;
    }

    return SUCCESS;
}

int switches_handle_sensor(char c) {

    if (reading != -1) {

        sensor_data[reading++] = c;

        if (reading == NUMBER_OF_SENSORS*SIZE_OF_SENSOR_DATA) {
            reading = -1;
        }

    }

    return SUCCESS;
}
