#pragma once

void s_timer();

int timer_enable();
int timer_update_ticks();
void timer_invoke();

inline int timer_get_50_ms();
