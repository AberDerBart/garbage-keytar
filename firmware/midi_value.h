#pragma once

#include <stdint.h>
#include <stdbool.h>

struct midi_value;

typedef struct midi_value{
  uint16_t value;
  uint16_t settings_value;
  uint16_t sent_value;
  bool force_send;
  uint16_t min_value;
  uint16_t max_value;
  uint16_t default_value;
  char* label;
  bool (*send)(struct midi_value* v);
} midi_value;

void midi_value_set(midi_value* v, uint16_t value, bool set_setting);
void midi_value_send_all();

extern midi_value mv_attack;
extern midi_value mv_decay;
extern midi_value mv_sustain;
extern midi_value mv_release;
