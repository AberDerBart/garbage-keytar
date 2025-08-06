#include "keymap.h"

#include <stddef.h>

#include "keyboard_matrix.h"
#include "ui/menu.h"
#include "ui/menu_items.h"
#include "ui/stack.h"

void keymap_action(keymap_t* keymap) {
  set_keymap(keymap);
  ui_clear();
}

void action_keymap_piano() { keymap_action(&keymap_piano); }
void action_keymap_accordion_c() { keymap_action(&keymap_accordion_c); };
void action_keymap_accordion_b() { keymap_action(&keymap_accordion_b); };
void action_keymap_accordion_6plus6() {
  keymap_action(&keymap_accordion_6plus6);
};
void action_keymap_wicki_hayden() { keymap_action(&keymap_wicki_hayden); };
void action_keymap_guitar() { keymap_action(&keymap_guitar); };

void push_menu_keymap() {
  ui_menu_t* menu = push_menu(6);
  menu_add(menu, make_default_menu_item("Piano", action_keymap_piano));
  menu_add(menu,
           make_default_menu_item("Accordion C", action_keymap_accordion_c));
  menu_add(menu,
           make_default_menu_item("Accordion B", action_keymap_accordion_b));
  menu_add(menu, make_default_menu_item("6+6", action_keymap_accordion_6plus6));
  menu_add(menu,
           make_default_menu_item("Wicki-Hayden", action_keymap_wicki_hayden));
  menu_add(menu, make_default_menu_item("Guitar", action_keymap_guitar));
}