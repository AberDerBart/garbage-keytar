#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "midi_value.h"

void analog_strip_init();

void analog_strip_assign(midi_value* mv, bool reset_on_release);
midi_value* analog_strip_get_assigned();
bool analog_strip_get_reset_on_release();

bool analog_strip_read_uint16(uint16_t* value);
bool analog_strip_read_float(float* value);

bool analog_strip_read(midi_value* mv, bool reset_on_release);

void analog_strip_task();
