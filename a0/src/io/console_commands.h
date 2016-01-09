#pragma once

#define COMMAND_TRAIN   0
#define COMMAND_REVERSE 1
#define COMMAND_SWITCH  2
#define COMMAND_HALT    3

#define COMMAND_TRAIN_CHAR_1 't'
#define COMMAND_TRAIN_CHAR_2 'r'

#define COMMAND_REVERSE_CHAR_1 'r'
#define COMMAND_REVERSE_CHAR_2 'v'

#define COMMAND_SWITCH_CHAR_1 's'
#define COMMAND_SWITCH_CHAR_2 'w'

#define COMMAND_HALT_CHAR_1 'q'

#define COMMAND_STATE_ACCEPT    0
#define COMMAND_STATE_TRAIN_1   1
#define COMMAND_STATE_REVERSE_1 2
#define COMMAND_STATE_SWITCH_1  3

struct command_t {
    int command;
    int arg1;
    int arg2;
};

typedef struct command_t command_t;

