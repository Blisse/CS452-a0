#include <error.h>

#include "circular_buffer.h"

inline void circular_buffer_init(circular_buffer_t* c_buffer) {
    c_buffer->size = CIRCULAR_BUFFER_SIZE;
    c_buffer->idx_start = 0;
    c_buffer->idx_end = 0;
}

inline int circular_buffer_empty(circular_buffer_t* c_buffer) {
    return (c_buffer->idx_start == c_buffer->idx_end);
}

inline int circular_buffer_full(circular_buffer_t* c_buffer) {
    return (c_buffer->idx_start == ((c_buffer->idx_end + 1) % c_buffer->size));
}

int circular_buffer_push(circular_buffer_t* c_buffer, char c) {

    if (circular_buffer_full(c_buffer)) {
        return E_FAIL;
    }

    c_buffer->buffer[c_buffer->idx_end] = c;

    c_buffer->idx_end = (c_buffer->idx_end + 1) % c_buffer->size;

    return SUCCESS;
}

int circular_buffer_get(circular_buffer_t* c_buffer, char* c) {

    if (circular_buffer_empty(c_buffer)) {
        return E_FAIL;
    }

    *c = c_buffer->buffer[c_buffer->idx_start];

    return SUCCESS;
}

int circular_buffer_pop(circular_buffer_t* c_buffer) {

    if (circular_buffer_empty(c_buffer)) {
        return E_FAIL;
    }

    c_buffer->buffer[c_buffer->idx_start] = '0';

    c_buffer->idx_start = (c_buffer->idx_start + 1) % c_buffer->size;

    return SUCCESS;
}
