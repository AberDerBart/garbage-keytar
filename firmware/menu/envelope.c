#include <stddef.h>

#include "../midi_value.h"
#include "ui/menu.h"
#include "ui/menu_items/midi_value.h"

void menu_add_envelope_entries(ui_menu_t* menu) {
  menu_add(menu, ui_menu_item_midi_value_new(&mv_attack));
  menu_add(menu, ui_menu_item_midi_value_new(&mv_decay));
  menu_add(menu, ui_menu_item_midi_value_new(&mv_sustain));
  menu_add(menu, ui_menu_item_midi_value_new(&mv_release));
}

void push_menu_envelope() {
  ui_menu_t* menu = push_menu(4);
  menu_add_envelope_entries(menu);
}
