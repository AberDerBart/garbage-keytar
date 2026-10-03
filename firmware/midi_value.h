#pragma once

#include <stdint.h>
#include <stdbool.h>

struct midi_value;

typedef struct midi_value{
  uint16_t value;
  uint16_t settings_value;
  uint16_t min_value;
  uint16_t max_value;
  uint16_t default_value;
  char* label;
  void (*on_change)(struct midi_value* v);
} midi_value;

bool midi_value_set(midi_value* v, uint16_t value, bool set_setting);

extern midi_value mv_attack;
extern midi_value mv_decay;
extern midi_value mv_sustain;
extern midi_value mv_release;
