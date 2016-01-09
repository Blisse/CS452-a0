#pragma once

#include <error.h>
#include <vararg.h>

#define COM1    0
#define COM2    1

#define FIFO_ON  1
#define FIFO_OFF 0

void pio_setup();


int pio_receive_buffer_empty(int flags);
int pio_transmit_buffer_empty(int flags);

int pio_receive_buffer_full(int flags);
int pio_transmit_buffer_full(int flags);

int pio_set_fifo(int channel, int state);
int pio_set_speed(int channel, int speed);

int _pio_write_char(int channel, char c);
int pio_flush(int channel);
int pio_put_char(int channel, char c);

void _pio_format(int channel, char* fmt, va_list va);
void pio_print_f(int channel, char* fmt, ...);
