#pragma once

#define SWITCH_UNKNOWN  'U'
#define SWITCH_STRAIGHT 'S'
#define SWITCH_CURVED   'C'

struct switch_t {
    int number;
    char position;
};
typedef struct switch_t switch_t;

void switches_reset();
void switches_init();
void sensors_init();
void s_switches();
int switches_set_direction(int sw, char direction);
int switches_set_curved();
char* switches_get_sensor();
int switches_request_sensor();
int switches_handle_sensor(char c);
