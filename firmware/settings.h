#pragma once
#include <stdint.h>

#include "keyboard_matrix.h"

extern int8_t offset;
extern uint8_t program;

int8_t offset_get_octave();
uint8_t offset_get_trans();
