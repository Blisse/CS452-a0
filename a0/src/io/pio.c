#include <ts7200.h>

#include <flags.h>
#include <numbers.h>
#include <common.h>
#include <error.h>

#include "pio.h"
#include "circular_buffer.h"

static circular_buffer_t com1_out_buffer;
static circular_buffer_t com2_out_buffer;
static circular_buffer_t com1_in_buffer;
static circular_buffer_t com2_in_buffer;

void pio_setup() {
    circular_buffer_init(&com1_out_buffer);
    circular_buffer_init(&com2_out_buffer);
    circular_buffer_init(&com1_in_buffer);
    circular_buffer_init(&com2_in_buffer);
}

int pio_receive_buffer_empty(int flags) {
    return are_all_set(flags, RXFE_MASK);
}

int pio_transmit_buffer_empty(int flags) {
    return are_all_set(flags, TXFE_MASK);
}

int pio_receive_buffer_full(int flags) {
    return are_all_set(flags, RXFF_MASK);
}

int pio_transmit_buffer_full(int flags) {
    return are_all_set(flags, TXFF_MASK);
}

circular_buffer_t* OUT_BUFFER(int channel) {
    if (channel == COM1) {
        return &com1_out_buffer;
    } else {
        return &com2_out_buffer;
    }
}

circular_buffer_t* IN_BUFFER(int channel) {
    if (channel == COM1) {
        return &com1_in_buffer;
    } else {
        return &com2_in_buffer;
    }
}

/*
 * The UARTs are initialized by RedBoot to the following state
 *  115,200 bps
 *  8 bits
 *  no parity
 *  fifos enabled
 */
int pio_set_fifo(int channel, int state) {
    int* line = ADDRESS_OF_UART(channel, UART_LCRH_OFFSET);

    if (state == FIFO_ON) {
        set_flags(line, FEN_MASK);
    } else {
        clear_flags(line, FEN_MASK);
    }

    return SUCCESS;
}

int pio_set_speed(int channel, int speed) {
    int* high = ADDRESS_OF_UART(channel, UART_LCRM_OFFSET);
    int* low = ADDRESS_OF_UART(channel, UART_LCRL_OFFSET);

    switch (speed) {
    case 115200:
        *high = 0x0;
        *low = 0x3;
        break;
    case 2400:
        *high = 0x0;
        *low = 0xBF;
        break;
    default:
        return E_FAIL;
        break;
    }

    return SUCCESS;
}

int pio_set_parity(int channel, int parity) {
    int* high = ADDRESS_OF_UART(channel, UART_LCRH_OFFSET);

    switch (parity) {
    case PARITY_ON:
        set_flags(high, PEN_MASK);
        break;
    case PARITY_OFF:
        clear_flags(high, PEN_MASK);
        break;
    default:
        return E_FAIL;
        break;
    }

    return SUCCESS;
}

int pio_set_wlen(int channel, int wlen) {
    int* high = ADDRESS_OF_UART(channel, UART_LCRH_OFFSET);

    switch (wlen) {
    case WLEN_8BIT:
        set_flags(high, WLEN_MASK);
        break;
    case WLEN_5BIT:
        clear_flags(high, WLEN_MASK);
        break;
    default:
        return E_FAIL;
        break;
    }

    return SUCCESS;
}

int pio_set_stop_bits(int channel, int stop_bits) {
    int* high = ADDRESS_OF_UART(channel, UART_LCRH_OFFSET);

    switch (stop_bits) {
    case STOP_BITS_1:
        clear_flags(high, STP2_MASK);
        break;
    case STOP_BITS_2:
        set_flags(high, STP2_MASK);
        break;
    default:
        return E_FAIL;
        break;
    }

    return SUCCESS;
}

int _pio_write_char(int channel, char c) {
    int* data = ADDRESS_OF_UART(channel, UART_DATA_OFFSET);

    *data = c;

    return SUCCESS;
}

int _pio_read_char(int channel, char* c) {
    int* data = ADDRESS_OF_UART(channel, UART_DATA_OFFSET);

    *c = *data;

    return SUCCESS;
}

int pio_fetch(int channel) {
    int* flags = ADDRESS_OF_UART(channel, UART_FLAG_OFFSET);
    circular_buffer_t* c_buffer = IN_BUFFER(channel);
    char c;

    if (pio_receive_buffer_full(*flags) && circular_buffer_full(c_buffer) == FALSE) {
        if (SUCCEEDED(_pio_read_char(channel, &c))) {
            circular_buffer_push(c_buffer, c);
        }
    }

    return SUCCESS;
}

int pio_flush(int channel) {
    int* flags = ADDRESS_OF_UART(channel, UART_FLAG_OFFSET);
    circular_buffer_t* c_buffer = OUT_BUFFER(channel);
    char c;

    while (!pio_transmit_buffer_full(*flags) && circular_buffer_empty(c_buffer) == FALSE) {
        if (SUCCEEDED(circular_buffer_get(c_buffer, &c))) {
            if (SUCCEEDED(_pio_write_char(channel, c))) {
                circular_buffer_pop(c_buffer);
            }
        }
    }

    return SUCCESS;
}

int pio_get_char(int channel, char* c) {

    if (channel == COM1) {
        if (circular_buffer_empty(&com1_in_buffer) == FALSE) {
            if (SUCCEEDED(circular_buffer_get(&com1_in_buffer, c))) {
                circular_buffer_pop(&com1_in_buffer);
                return SUCCESS;
            }
        }
    } else {
        if (circular_buffer_empty(&com2_in_buffer) == FALSE) {
            if (SUCCEEDED(circular_buffer_get(&com2_in_buffer, c))) {
                circular_buffer_pop(&com2_in_buffer);
                return SUCCESS;
            }
        }
    }

    return E_FAIL;
}


int pio_put_char(int channel, char c) {

    if (channel == COM1) {
        return circular_buffer_push(&com1_out_buffer, c);
    } else {
        return circular_buffer_push(&com2_out_buffer, c);
    }

}

int pio_put_char_array(int channel, char* buffer, int buffer_size) {
    int n = buffer_size;
    char c;

    while ((c = *(buffer++))) {

        if (channel == COM1) {
            circular_buffer_push(&com1_out_buffer, c);
        } else {
            circular_buffer_push(&com2_out_buffer, c);
        }

        n = n - 1;
        if (n == 0) {
            return E_FAIL;
        }
    }

    return SUCCESS;
}

void _pio_format(int channel, char* fmt, va_list va) {
    char buffer[256];
    char ch;

    while ((ch = *(fmt++))) {

        if (ch != '%') {
            pio_put_char(channel, ch);
        } else {
            ch = *(fmt++);

            switch (ch) {
            case 'c':
                pio_put_char(channel, va_arg(va, char));
                break;
            case 's':
                pio_put_char_array(channel, va_arg(va, char*), 256);
                break;
            case 'u':
                put_unsigned_int_in_buffer(va_arg(va, unsigned int), buffer, 256);
                pio_put_char_array(channel, buffer, 256);
                break;
            case 'd':
                put_int_in_buffer(va_arg(va, int), buffer, 256);
                pio_put_char_array(channel, buffer, 256);
                break;
            }

        }
    }
}

void pio_print_f(int channel, char* fmt, ...) {
    va_list va;

    va_start(va, fmt);

    _pio_format(channel, fmt, va);

    va_end(va);
}
