#pragma once

void s_trains_controller();

void trains_controller_on_50ms();
void trains_controller_on_250ms();

int trains_controller_send_char(char c);
int trains_controller_send_chars(char c_1, char c_2);
int trains_controller_send_pause();
