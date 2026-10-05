#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "midi_value.h"

void analog_strip_init();

bool analog_strip_read_uint16(uint16_t* value);
bool analog_strip_read_float(float* value);

bool analog_strip_read(midi_value* mv, bool reset_on_release);
