#pragma once

void s_timer();

int timer_enable();
int timer_update_ticks();
void timer_invoke();

void timer_invoke_on_50ms();
void timer_invoke_on_100ms();
void timer_invoke_on_250ms();

inline unsigned int timer_get_25_ms();
