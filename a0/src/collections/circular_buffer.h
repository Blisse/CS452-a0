#pragma once

#define CIRCULAR_BUFFER_SIZE 2056;

struct circular_buffer_t {
    char buffer[2056];
    int size;
    int idx_start;
    int idx_end;
};

typedef struct circular_buffer_t circular_buffer_t;

inline void circular_buffer_init(circular_buffer_t* c_buffer);
inline int circular_buffer_empty(circular_buffer_t* c_buffer);
inline int circular_buffer_full(circular_buffer_t* c_buffer);
inline int circular_buffer_has_room(circular_buffer_t* c_buffer, int room);
int circular_buffer_push(circular_buffer_t* c_buffer, char c);
int circular_buffer_get(circular_buffer_t* c_buffer, char* c);
int circular_buffer_pop(circular_buffer_t* c_buffer);
