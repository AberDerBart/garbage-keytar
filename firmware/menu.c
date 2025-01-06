#include <menu.h>
#include <stddef.h>

#include "ble_midi_client.h"
#include "display.h"
#include "keyboard_matrix.h"
#include "menu/bluetooth.h"
#include "midi_ble.h"

menu_item_t menu_item_main;

menu_item_t mi_keymap;

menu_item_t mi_keymap_piano = {
  label : "Piano",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_piano
};

menu_item_t mi_keymap_accordion_c = {
  label : "C-Accordion",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_accordion_c
};

menu_item_t mi_keymap_accordion_b = {
  label : "B-Accordion",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_accordion_b,
};

menu_item_t mi_keymap_accordion_6_plus_6 = {
  label : "6+6",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_accordion_6plus6,
};

menu_item_t mi_keymap_wicki_hayden = {
  label : "Wicki-Hayden",
  parent : &mi_keymap,
  children : NULL,
  action : set_keymap_wicki_hayden,
};

menu_item_t* mi_children_keymap[] = {
    &mi_keymap_piano,        &mi_keymap_accordion_b,
    &mi_keymap_accordion_c,  &mi_keymap_accordion_6_plus_6,
    &mi_keymap_wicki_hayden, NULL,
};

menu_item_t mi_keymap = {
  label : "Keymap",
  parent : &menu_item_main,
  children : mi_children_keymap,
  action : NULL,
};

menu_item_t* menu_children[] = {
    &mi_bluetooth,
    &mi_keymap,
    NULL,
};

menu_item_t menu_item_main = {
  label : NULL,
  parent : NULL,
  children : menu_children,
  action : NULL,
};

bool menu_update() { return menu_update_bluetooth(); }