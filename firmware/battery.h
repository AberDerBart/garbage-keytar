#pragma once

#include <stdbool.h>

void battery_init();

bool battery_is_charging();

bool battery_is_fully_charged();

typedef enum {
  EMPTY,
  LOW,
  MEDIUM,
  FULL,
} battery_level_t;

float battery_get_voltage();

battery_level_t battery_get_level();