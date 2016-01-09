#pragma once

#define CIRCULAR_BUFFER_SIZE 256;

struct circular_buffer_t {
    char buffer[256];
    int size;
    int idx_start;
    int idx_end;
};

typedef struct circular_buffer_t circular_buffer_t;

inline void circular_buffer_init(circular_buffer_t* c_buffer);
inline int circular_buffer_empty(circular_buffer_t* c_buffer);
inline int circular_buffer_full(circular_buffer_t* c_buffer);
int circular_buffer_push(circular_buffer_t* c_buffer, char c);
int circular_buffer_get(circular_buffer_t* c_buffer, char* c);
int circular_buffer_pop(circular_buffer_t* c_buffer);
