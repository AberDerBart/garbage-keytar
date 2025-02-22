#include <menu.h>
#include <stddef.h>

#include "ble_midi_client.h"
#include "display.h"
#include "keyboard_matrix.h"
#include "menu/bluetooth.h"
#include "menu/envelope.h"
#include "menu/keymap.h"
#include "midi_ble.h"

menu_item_t menu_item_main;

menu_item_t* menu_children[] = {
    &mi_bluetooth,
    &mi_keymap,
    &mi_envelope,
    NULL,
};

menu_item_t menu_item_main = {
  label : NULL,
  parent : NULL,
  children : menu_children,
  action : NULL,
  change : NULL,
  get_value : NULL,
};

bool menu_update() { return menu_update_bluetooth(); }