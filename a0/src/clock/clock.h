#pragma once

#define MODE_FREE_RUNNING   0
#define MODE_PERIODIC       1

#define CLKSEL_2KHZ    0
#define CLKSEL_508KHZ  1

void clock_setup();

int timer_enable();
int timer_update();
int timer_get_ticks();

int clock_update();
int clock_get_hundred_ms();
