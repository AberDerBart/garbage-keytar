#include <stddef.h>

#include "./bluetooth_connect.h"
#include "midi_ble.h"
#include "midi_ble_client.h"
#include "ui/menu.h"
#include "ui/menu_items.h"
#include "ui_stack.h"

void disconnect() {
  midi_ble_client_disconnect();
  ui_clear();
}

void push_menu_bluetooth() {
  ui_menu_t* menu = push_menu(3);
  if (midi_ble_client_is_initialized()) {
    if (midi_ble_client_is_connected()) {
      menu_add(menu, make_default_menu_item("Disconnect", disconnect));
    } else {
      menu_add(menu,
               make_default_menu_item("Connect", push_menu_bluetooth_connect));
    }
    menu_add(menu, make_default_menu_item("Remember connection",
                                          midi_ble_set_state_as_initial));
    menu_add(menu, make_default_menu_item("Disable", midi_ble_client_deinit));
    return;
  }

  if (midi_ble_server_is_initialized()) {
    menu_add(menu, make_default_menu_item("Disable", midi_ble_deinit));
    return;
  }

  menu_add(menu, make_default_menu_item("Start server", midi_ble_server_init));
  menu_add(menu, make_default_menu_item("Start client", midi_ble_client_init));
}
