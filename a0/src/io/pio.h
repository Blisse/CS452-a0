#pragma once

#include <error.h>
#include <vararg.h>

#define COM1    0
#define COM2    1

#define FIFO_OFF 0
#define FIFO_ON  1

#define PARITY_OFF  0
#define PARITY_ON   1

#define STOP_BITS_1 0
#define STOP_BITS_2 1

#define WLEN_8BIT   4
#define WLEN_5BIT   1

void s_pio();

int pio_receive_buffer_empty(int flags);
int pio_transmit_buffer_empty(int flags);

int pio_receive_buffer_full(int flags);
int pio_transmit_buffer_full(int flags);


int pio_set_fifo(int channel, int state);
int pio_set_speed(int channel, int speed);
int pio_set_parity(int channel, int parity);
int pio_set_stop_bits(int channel, int stop_bits);
int pio_set_wlen(int channel, int wlen);

int _pio_write_char(int channel, char c);

int pio_fetch(int channel);
int pio_flush(int channel);
int pio_get_char(int channel, char* c);
int pio_put_char(int channel, char c);

void _pio_format(int channel, char* fmt, va_list va);
void pio_print_f(int channel, char* fmt, ...);
