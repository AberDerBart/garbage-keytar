#include <stddef.h>

#include "../adsr.h"
#include "keymap.h"

menu_item_t mi_envelope;

menu_item_t mi_attack = {
  label : "Attack",
  parent : &mi_envelope,
  children : NULL,
  action : NULL,
  change : change_attack_setting,
  get_value : adsr_menu_get_attack,
};

menu_item_t mi_decay = {
  label : "Decay",
  parent : &mi_envelope,
  children : NULL,
  action : NULL,
  change : change_decay_setting,
  get_value : adsr_menu_get_decay,
};

menu_item_t mi_sustain = {
  label : "Sustain",
  parent : &mi_envelope,
  children : NULL,
  action : NULL,
  change : change_sustain_setting,
  get_value : adsr_menu_get_sustain,
};

menu_item_t mi_release = {
  label : "Release",
  parent : &mi_envelope,
  children : NULL,
  action : NULL,
  change : change_release_setting,
  get_value : adsr_menu_get_release,
};

menu_item_t* mi_children_envelope[] = {
    &mi_attack, &mi_decay, &mi_sustain, &mi_release, NULL,
};

menu_item_t mi_envelope = {
  label : "Envelope",
  parent : &menu_item_main,
  children : mi_children_envelope,
  action : NULL,
  change : NULL,
  get_value : NULL,
};