#pragma once

#include "switches.h"

void pretty_move_cursor(int row, int column);

void pretty_print_header();

void pretty_print_clock(int hundred_ms);

void pretty_print_switches(switch_t* switches, int size, int sw, char dir);

void pretty_print_command(char* command, int size);
