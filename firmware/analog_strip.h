#pragma once
#include <stdbool.h>
#include <stdint.h>

void analog_strip_init();

bool analog_strip_read_uint16(uint16_t* value);
bool analog_strip_read_float(float* value);