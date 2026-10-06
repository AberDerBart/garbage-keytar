#include "./main.h"

#include "./bluetooth.h"
#include "./envelope.h"
#include "keyboard_matrix.h"
#include "ui/menu.h"
#include "ui/menu_items/select.h"
#include "ui/menu_items/checkbox.h"
#include "ui/menu_items.h"
#include "midi_value.h"
#include "analog_strip.h"

char* get_keymap_label(void* option) {
  keymap_t* keymap = option;
  return keymap->label;
}

void set_keymap_from_menu(void* option) {
  keymap_t* keymap = option;
  set_keymap(keymap);
}

char* get_midi_value_label(void* option) {
  midi_value* mv = option;
  return mv->label;
}

void set_analog_strip_mv(void* option) {
  midi_value* mv = option;
  analog_strip_assign(mv, analog_strip_get_reset_on_release());
}

void set_analog_strip_reset_on_release(bool reset) {
  analog_strip_assign(analog_strip_get_assigned(), reset);
}

void push_menu_main() {
  ui_menu_t* menu = push_menu(6);
  menu_add(menu, make_default_menu_item("Bluetooth", push_menu_bluetooth));
  menu_add(menu, ui_menu_item_select_new("Keymap", (void**) keymaps, get_keymap_index(), &get_keymap_label, set_keymap_from_menu));
  menu_add(menu, make_default_menu_item("Envelope", push_menu_envelope));

  midi_value* analog_strip_mv = analog_strip_get_assigned();
  size_t analog_strip_index = 0;
  for (size_t i = 0; midi_values[i]; i++) {
    if (midi_values[i] == analog_strip_mv){
      analog_strip_index = i;
    }
  }
  menu_add(menu, ui_menu_item_select_new("Strip control", (void**) midi_values, analog_strip_index, &get_midi_value_label, set_analog_strip_mv));
  menu_add(menu, ui_menu_item_checkbox_new("Reset on release", analog_strip_get_reset_on_release(), &set_analog_strip_reset_on_release)); 
}
