#pragma once

#define COMMAND_CHAR_TRAIN_1    't'
#define COMMAND_CHAR_TRAIN_2    'r'

#define COMMAND_CHAR_REVERSE_1  'r'
#define COMMAND_CHAR_REVERSE_2  'v'

#define COMMAND_CHAR_SWITCH_1   's'
#define COMMAND_CHAR_SWITCH_2   'w'
#define COMMAND_CHAR_SWITCH_STRAIGHT    'S'
#define COMMAND_CHAR_SWITCH_CURVED      'C'

#define COMMAND_CHAR_HALT_1     'q'

#define COMMAND_CHAR_SPACE      ' '

#define COMMAND_CHAR_DONE_1     '\r'
#define COMMAND_CHAR_DONE_2     '\n'

void s_cio();

void cio_buffer_reset();

void cio_read();

int cio_update_console(char c);

int cio_send_command(char* buffer, int buffer_size);
int cio_send_train_command(char* buffer, int buffer_size);
int cio_send_reverse_command(char* buffer, int buffer_size);
int cio_send_switch_command(char* buffer, int buffer_size);
int cio_send_halt_command(char* buffer, int buffer_size);

int cio_quit();
