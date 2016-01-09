#include <ts7200.h>

#include <flags.h>
#include <numbers.h>

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

/*
 * The UARTs are initialized by RedBoot to the following state
 *  115,200 bps
 *  8 bits
 *  no parity
 *  fifos enabled
 */
int pio_set_fifo(int channel, int state) {
    int* line = ADDRESS_OF_UART(channel, UART_LCRH_OFFSET);
    int line_val = *line;

    if (state == FIFO_ON) {
        line_val |= FEN_MASK;
    } else {
        line_val &= ~FEN_MASK;
    }

    *line = line_val;

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
        *low = 0x90;
        break;
    default:
        return E_FAIL;
        break;
    }

    return SUCCESS;
}

int _pio_write_c(int channel, char c) {
    int* flags = ADDRESS_OF_UART(channel, UART_FLAG_OFFSET);
    int* data = ADDRESS_OF_UART(channel, UART_DATA_OFFSET);

    if (*flags & TXFF_MASK) {
        return E_FAIL;
    }

    *data = c;

    return SUCCESS;
}

int _pio_read_c(int channel, char* c) {
    int* flags = ADDRESS_OF_UART(channel, UART_FLAG_OFFSET);
    int* data = ADDRESS_OF_UART(channel, UART_DATA_OFFSET);

    if (!(*flags & RXFF_MASK)) {
        return E_FAIL;
    }

    *c = *data;

    return SUCCESS;
}

int pio_fetch(int channel) {
    int* flags = ADDRESS_OF_UART(channel, UART_FLAG_OFFSET);
    char c;

    circular_buffer_t* c_buffer;
    if (channel == COM1) {
        c_buffer = &com1_in_buffer;
    } else {
        c_buffer = &com2_in_buffer;
    }

    while ((*flags & RXFF_MASK) && !circular_buffer_full(c_buffer)) {
        if (_pio_read_c(channel, &c)) {
            circular_buffer_push(c_buffer, c);
        }
    }

    return SUCCESS;
}

int pio_flush(int channel) {
    int* flags = ADDRESS_OF_UART(channel, UART_FLAG_OFFSET);
    char c;

    circular_buffer_t* c_buffer;
    if (channel == COM1) {
        c_buffer = &com1_out_buffer;
    } else {
        c_buffer = &com2_out_buffer;
    }

    while (!(*flags & TXFF_MASK) && !circular_buffer_empty(c_buffer)) {
        if (circular_buffer_get(c_buffer, &c) == SUCCESS) {
            if (_pio_write_c(channel, c) == SUCCESS) {
                circular_buffer_pop(c_buffer);
            }
        }
    }

    return SUCCESS;
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

int pio_get_char(int channel, char* c) {

    if (channel == COM1) {
        if (!circular_buffer_empty(&com1_out_buffer)) {
            circular_buffer_get(&com1_out_buffer, c);
        } else {
            return E_FAIL;
        }
    } else {
        if (!circular_buffer_empty(&com2_out_buffer)) {
            circular_buffer_get(&com2_out_buffer, c);
        } else {
            return E_FAIL;
        }
    }

    return SUCCESS;
}

void _pio_format(int channel, char* fmt, va_list va) {
    char buffer[12];
    char ch;
    int min_digits;

    while ((ch = *(fmt++))) {

        if (ch != '%') {
            pio_put_char(channel, ch);
        } else {
            ch = *(fmt++);

            min_digits = 0;
            switch (ch) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                min_digits = (ch - '0');
                break;
            }

            switch (ch) {
            case 'u':
                put_unsigned_int_in_buffer(va_arg(va, unsigned int), buffer, 12);
                pio_put_char_array(channel, buffer, 12);
                break;
            case 'd':
                put_int_in_buffer(va_arg(va, int), buffer, 12);
                pio_put_char_array(channel, buffer, 12);
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
