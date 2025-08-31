#include "./main.h"

#include "./bluetooth.h"
#include "./envelope.h"
#include "./keymap.h"
#include "./profiles.h"
#include "keyboard_matrix.h"
#include "ui/menu.h"
#include "ui/menu/select.h"
#include "ui/menu_items.h"
#include "ui/stack.h"

char* keymap_options[N_KEYMAPS + 1];

void push_menu_main() {
  ui_menu_t* menu = push_menu(4);
  menu_add(menu, make_default_menu_item("Bluetooth", push_menu_bluetooth));

  for (size_t i = 0; i < N_KEYMAPS; i++) {
    keymap_options[i] = keymaps[i]->label;
  }
  keymap_options[N_KEYMAPS] = NULL;
  menu_add(menu, ui_menu_item_select_new("Keymap", get_keymap_index(),
                                         keymap_options, set_keymap_by_index));

  menu_add(menu, make_default_menu_item("Envelope", push_menu_envelope));
  menu_add(menu, make_default_menu_item("Profiles", push_menu_profiles));
}