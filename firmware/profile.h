#pragma once
#include <stdint.h>

#include "adsr.h"
#include "midi_ble.h"

typedef struct profile_t {
  uint8_t keymap_index;
  adsr_settings_t adsr;
  ble_state_t ble;
} profile_t;

extern profile_t profiles[12];

void profile_apply(profile_t* profile);
