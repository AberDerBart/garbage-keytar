#include <menu.h>
#include <stddef.h>

#include "display.h"

menu_item_t menu_item_main;

menu_item_t menu_item_bluetooth = {
  label : "Bluetooth",
  parent : &menu_item_main,
  children : NULL,
};

menu_item_t menu_item_keymap;

menu_item_t menu_item_keymap_piano = {
  label : "Piano",
  parent : &menu_item_keymap,
  children : NULL,
};

menu_item_t menu_item_keymap = {
  label : "Keymap",
  parent : &menu_item_main,
  children : {&menu_item_keymap_piano, NULL},
};

menu_item_t menu_item_main = {
  label : NULL,
  parent : NULL,
  children : {
      &menu_item_bluetooth,
      &menu_item_keymap,
      NULL,
  },
};
