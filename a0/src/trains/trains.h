#pragma once

void train_setup();
int train_set_speed(int train, int speed);
int train_set_special_speed(int train, int speed);
int train_set_reverse(int train);
int track_set_switch(int track_switch, char direction);
int train_update();
