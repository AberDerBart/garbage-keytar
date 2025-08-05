#include "./main.h"

#include "./bluetooth.h"
#include "./envelope.h"
#include "./keymap.h"
#include "ui/menu.h"
#include "ui/menu_items.h"
#include "ui_stack.h"

void push_menu_main() {
  ui_menu_t* menu = push_menu(3);
  menu_add(menu, make_default_menu_item("Bluetooth", push_menu_bluetooth));
  menu_add(menu, make_default_menu_item("Keymap", push_menu_keymap));
  menu_add(menu, make_default_menu_item("Envelope", push_menu_envelope));
}