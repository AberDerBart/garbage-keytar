#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

void keyboard_init();

void keyboard_task();

bool keyboard_get_connected();

#endif
