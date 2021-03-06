#pragma once

static inline int are_all_set(int flag, unsigned int mask) {
    return ((flag & mask) == mask);
}

static inline int is_any_set(int flag, unsigned int mask) {
    return (flag & mask);
}

static inline void set_flags(int* flag, unsigned int mask) {
    *flag = (*flag) | mask;
}

static inline void clear_flags(int* flag, unsigned int mask) {
    *flag = (*flag) & ~mask;
}
