#include <stddef.h>

#include "../adsr.h"
#include "ui/menu.h"
#include "ui/menu_items.h"

void push_menu_envelope() {
  ui_menu_t* menu = push_menu(4);
  menu_add(menu, ui_menu_item_uint8_new("Attack", &adsr_settings.attack, 0, 127,
                                        NULL));

  menu_add(menu,
           ui_menu_item_uint8_new("Decay", &adsr_settings.decay, 0, 127, NULL));

  menu_add(menu, ui_menu_item_uint8_new("Sustain", &adsr_settings.sustain, 0,
                                        127, NULL));

  menu_add(menu, ui_menu_item_uint8_new("Release", &adsr_settings.release, 0,
                                        127, NULL));
}