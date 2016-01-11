#pragma once


struct train_t {
    int number;
    int status;
    int speed;
};
typedef struct train_t train_t;

void s_trains();

void trains_init();

int train_go();
int train_stop();
int train_set_speed(int train, int speed);
int train_set_reverse(int train);
int track_set_switch(int track_switch, char direction);
