#include "midi_value.h"
#include "midi.h"
#include "util.h"

#include <stddef.h>

#define MIDI_VALUE(NAME, LABEL, DEF, MIN, MAX, ON_CHANGE) midi_value NAME = { .label = LABEL, .min_value = MIN, .max_value = MAX, .value = DEF, .default_value = DEF, .settings_value = DEF, .sent_value = DEF, .force_send = true, .send = ON_CHANGE };

bool midi_value_sync(midi_value *mv) {
  if (!mv->force_send && mv->value == mv->sent_value) {
    return true;
  }

  if (!mv->send(mv)) {
    return false;
  }

  mv->force_send = false;
  mv->sent_value = mv->value;

  return true;
}

void midi_value_set(midi_value *mv, uint16_t value, bool set_setting) {
  uint16_t clamped = clamp_u16(value, mv->min_value, mv->max_value);

  mv->value = clamped;
  if(set_setting) {
    mv->settings_value = clamped;
  }
  midi_value_sync(mv);
}

bool send_attack(midi_value *v) {
  return midi_cc_attack(v->value);
}
MIDI_VALUE(mv_attack, "Attack", 0, 0, 127, send_attack)

bool send_decay(midi_value *v) {
  return midi_cc_decay(v->value);
}
MIDI_VALUE(mv_decay, "Decay", 0, 0, 127, send_decay)

bool send_sustain(midi_value *v) {
  return midi_cc_sustain(v->value);
}
MIDI_VALUE(mv_sustain, "Sustain", 0, 0, 127, send_sustain)

bool send_release(midi_value *v) {
  return midi_cc_release(v->value);
}
MIDI_VALUE(mv_release, "Release", 0, 0, 127, send_release)

midi_value* midi_values[] = {&mv_attack, &mv_decay, &mv_sustain, &mv_release, NULL};

void midi_value_send_all() {
  for (midi_value** mv = &midi_values[0]; *mv != NULL; mv++) {
    (*mv)->force_send = true;
    midi_value_sync((*mv));
  }
}

