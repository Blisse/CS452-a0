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
void switch_setup();
