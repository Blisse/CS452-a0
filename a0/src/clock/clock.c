#include "clock.h"

#include "timer.h"
#include "pretty_io.h"

void s_clock() {
}

void clock_on_100ms() {
    pretty_print_clock(timer_get_50_ms() * 2);
}
