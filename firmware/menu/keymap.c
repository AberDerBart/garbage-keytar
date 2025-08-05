#include "keymap.h"

#include <stddef.h>

#include "keyboard_matrix.h"
#include "ui_menu.h"
#include "ui_stack.h"

typedef struct menu_item_keymap_t {
  menu_item_t base;
  keymap_t* keymap;
} menu_item_keymap_t;

void select_keymap_action(menu_item_t* mi) {
  menu_item_keymap_t* self = (menu_item_keymap_t*)mi;
  set_keymap(self->keymap);
  ui_clear();
}

menu_item_keymap_t mi_keymap_piano = {
  base : {
    label : "Piano",
    action : select_keymap_action,
  },
  keymap : &keymap_piano,
};

menu_item_keymap_t mi_keymap_accordion_c = {
  base : {
    label : "C-Accordion",
    action : select_keymap_action,
  },
  keymap : &keymap_accordion_c,
};

menu_item_keymap_t mi_keymap_accordion_b = {
  base : {
    label : "B-Accordion",
    action : select_keymap_action,
  },
  keymap : &keymap_accordion_b,
};

menu_item_keymap_t mi_keymap_accordion_6plus6 = {
  base : {
    label : "6+6",
    action : select_keymap_action,
  },
  keymap : &keymap_accordion_6plus6,
};

menu_item_keymap_t mi_keymap_wicki_hayden = {
  base : {
    label : "Wicki-Hayden",
    action : select_keymap_action,
  },
  keymap : &keymap_wicki_hayden,
};

menu_item_keymap_t mi_keymap_guitar = {
  base : {
    label : "Guitar",
    action : select_keymap_action,
  },
  keymap : &keymap_guitar,
};

menu_item_keymap_t* mi_children_keymap[] = {
    &mi_keymap_piano,
    &mi_keymap_accordion_b,
    &mi_keymap_accordion_c,
    &mi_keymap_accordion_6plus6,
    &mi_keymap_wicki_hayden,
    &mi_keymap_guitar,
    NULL,
};

void push_menu_keymap() { push_menu((menu_item_t**)mi_children_keymap); }