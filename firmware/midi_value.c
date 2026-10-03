#include "midi_value.h"
#include "midi.h"
#include "util.h"

bool midi_value_set(midi_value *mv, uint16_t value, bool set_setting) {
  uint16_t clamped = clamp_u16(value, mv->min_value, mv->max_value);
  if (clamped == mv->value && (!set_setting || mv->settings_value == clamped)) {
    return false;
  }

  mv->value = clamped;
  if(set_setting) {
    mv->settings_value = clamped;
  }
  mv->on_change(mv);
  return true;
}

void on_attack_change(midi_value *v) {
  midi_cc_attack(v->value);
}

midi_value mv_attack = {
  .value = 0,
  .settings_value = 0,
  .min_value = 0,
  .max_value = 127,
  .default_value = 0,
  .on_change = &on_attack_change,
};

void on_decay_change(midi_value *v) {
  midi_cc_decay(v->value);
}

midi_value mv_decay = {
  .value = 0,
  .settings_value = 0,
  .min_value = 0,
  .max_value = 127,
  .default_value = 0,
  .on_change = &on_decay_change,
};

void on_sustain_change(midi_value *v) {
  midi_cc_sustain(v->value);
}

midi_value mv_sustain = {
  .value = 0,
  .settings_value = 0,
  .min_value = 0,
  .max_value = 127,
  .default_value = 0,
  .on_change = &on_sustain_change,
};

void on_release_change(midi_value *v) {
  midi_cc_release(v->value);
}

midi_value mv_release = {
  .value = 0,
  .settings_value = 0,
  .min_value = 0,
  .max_value = 127,
  .default_value = 0,
  .on_change = &on_release_change,
};
