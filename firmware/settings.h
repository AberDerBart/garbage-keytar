#pragma once
#include <stdint.h>

#include "keyboard_matrix.h"

extern int8_t offset;
extern uint8_t program;
extern const key_mapping_t *key_mapping;
int8_t offset_get_octave();
uint8_t offset_get_trans();

void set_key_mapping(uint8_t index);
