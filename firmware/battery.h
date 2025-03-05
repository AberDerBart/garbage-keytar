#pragma once

#include <stdbool.h>

void battery_init();

bool battery_is_charging();

bool battery_is_fully_charged();

typedef enum {
  EMPTY = 0,
  LOW = 1,
  MEDIUM = 2,
  FULL = 3,
} battery_level_t;

float battery_get_voltage();

battery_level_t battery_get_level();