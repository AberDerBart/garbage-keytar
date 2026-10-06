#include "./main.h"

#include "./bluetooth.h"
#include "./envelope.h"
#include "keyboard_matrix.h"
#include "ui/menu.h"
#include "ui/menu_items/select.h"
#include "ui/menu_items.h"

char* get_keymap_label(void* option) {
  keymap_t* keymap = option;
  return keymap->label;
}

void set_keymap_from_menu(void* option) {
  keymap_t* keymap = option;
  set_keymap(keymap);
}

void push_menu_main() {
  ui_menu_t* menu = push_menu(4);
  menu_add(menu, make_default_menu_item("Bluetooth", push_menu_bluetooth));
  menu_add(menu, ui_menu_item_select_new("Keymap", (void**) keymaps, get_keymap_index(), &get_keymap_label, set_keymap_from_menu));
  menu_add(menu, make_default_menu_item("Envelope", push_menu_envelope));
}
