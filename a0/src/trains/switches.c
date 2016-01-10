#include "switches.h"

// Actually 160
#define NUMBER_OF_SWITCHES_SUPPORTED  160

static switch_t switches[NUMBER_OF_SWITCHES_SUPPORTED];

void switches_reset() {
    int i;

    for (i = 0; i < NUMBER_OF_SWITCHES_SUPPORTED; i++) {
        switches[i].number = -1;
        switches[i].position = SWITCH_UNKNOWN;
    }
}

void switches_init() {
    switches_reset();

    switches[1].number = 1;
    switches[2].number = 2;
    switches[3].number = 3;
    switches[4].number = 4;
    switches[5].number = 5;
    switches[6].number = 6;
    switches[7].number = 7;
    switches[8].number = 8;
    switches[9].number = 9;
    switches[10].number = 10;
    switches[11].number = 11;
    switches[12].number = 12;
    switches[13].number = 13;
    switches[14].number = 14;
    switches[15].number = 15;
    switches[16].number = 16;
    switches[17].number = 17;
    switches[18].number = 18;
    switches[153].number = 153;
    switches[154].number = 154;
    switches[155].number = 155;
    switches[156].number = 156;
}

void switch_setup() {
    switches_init();
}
