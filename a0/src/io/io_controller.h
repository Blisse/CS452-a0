#pragma once

void s_io_controller();

void io_controller_on_ms();

void io_controller_fetch();

void io_controller_flush();

inline int io_controller_running();

void io_controller_quit();
