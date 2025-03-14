#include "keymap.h"

#include <stddef.h>

#include "keyboard_matrix.h"

menu_item_t mi_keymap;

menu_item_t mi_keymap_piano = {
  label : "Piano",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_piano,
  change : NULL,
  get_value : NULL,
};

menu_item_t mi_keymap_accordion_c = {
  label : "C-Accordion",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_accordion_c,
  change : NULL,
  get_value : NULL,
};

menu_item_t mi_keymap_accordion_b = {
  label : "B-Accordion",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_accordion_b,
  change : NULL,
  get_value : NULL,
};

menu_item_t mi_keymap_accordion_6_plus_6 = {
  label : "6+6",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_accordion_6plus6,
  change : NULL,
  get_value : NULL,
};

menu_item_t mi_keymap_wicki_hayden = {
  label : "Wicki-Hayden",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_wicki_hayden,
  change : NULL,
  get_value : NULL,
};

menu_item_t mi_keymap_guitar = {
  label : "Guitar",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_guitar,
  change : NULL,
  get_value : NULL,
};

menu_item_t* mi_children_keymap[] = {
    &mi_keymap_piano,
    &mi_keymap_accordion_b,
    &mi_keymap_accordion_c,
    &mi_keymap_accordion_6_plus_6,
    &mi_keymap_wicki_hayden,
    &mi_keymap_guitar,
    NULL,
};

menu_item_t mi_keymap = {
  label : "Keymap",
  parent : &menu_item_main,
  children : mi_children_keymap,
  action : NULL,
  change : NULL,
  get_value : NULL,
};