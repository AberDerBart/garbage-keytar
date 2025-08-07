#include <stddef.h>

#include "../adsr.h"
#include "ui/menu.h"
#include "ui/menu_items.h"

void menu_add_envelope_entries(ui_menu_t* menu, adsr_settings_t* envelope) {
  menu_add(menu,
           ui_menu_item_uint8_new("Attack", &envelope->attack, 0, 127, NULL));

  menu_add(menu,
           ui_menu_item_uint8_new("Decay", &envelope->decay, 0, 127, NULL));

  menu_add(menu,
           ui_menu_item_uint8_new("Sustain", &envelope->sustain, 0, 127, NULL));

  menu_add(menu,
           ui_menu_item_uint8_new("Release", &envelope->release, 0, 127, NULL));
}

void push_menu_envelope() {
  ui_menu_t* menu = push_menu(4);
  menu_add_envelope_entries(menu, &adsr_settings);
}