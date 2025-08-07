#include "profile.h"

#include <string.h>

#include "adsr.h"
#include "keyboard_matrix.h"

profile_t profiles[12];

void profile_apply(profile_t* profile) {
  set_keymap_by_index(profile->keymap_index);
  memcpy(&adsr_settings, &(profile->adsr), sizeof(adsr_settings));
  adsr_send_midi();
}