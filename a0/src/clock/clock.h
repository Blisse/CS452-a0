#pragma once

void clock_setup();

void clock_update();
int clock_update_ticks();
int clock_get_hundred_ms();
int clock_tick(int hundred_ms_elapsed);
