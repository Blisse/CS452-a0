#pragma once

// Most functions here referenced from bwio.c

static inline int is_number(char ch) {
    return (ch >= '0' && ch <= '9');
}

static inline int is_alpha(char ch) {
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

static inline int char_to_int(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return -1;
}

static inline int char_array_to_int(char* ch, int* steps) {
    int i = 0;
    int n = 0;

    while (is_number(*ch)) {
        n *= 10;
        n += (int)(*ch - '0');
        i++;
        ch++;
    }

    *steps = i;

    return n;
}

static inline void put_unsigned_int_in_buffer(unsigned int num, char* buffer, int buffer_size) {
    int n = 0;
    int digit;
    unsigned int d = 1;

    while ((num / d) >= 10) {
        d *= 10;
    }

    while (d != 0) {
        digit = num / d;
        num %= d;
        d /= 10;
        if (n || digit > 0 || d == 0) {
            *buffer++ = digit + (digit < 10 ? '0' : 'a' - 10);
            n = n + 1;
            if (n == buffer_size - 1) {
                break;
            }
        }
    }
    *buffer = 0;
}

static inline void put_int_in_buffer(int num, char* buffer, int buffer_size) {
    if (num < 0) {
        num = -num;
        *buffer++ = '-';
        put_unsigned_int_in_buffer(num, buffer, buffer_size - 1);
    } else {
        put_unsigned_int_in_buffer(num, buffer, buffer_size);
    }
}
