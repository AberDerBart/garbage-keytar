#include <stddef.h>

#include "./bluetooth_connect.h"
#include "midi_ble.h"
#include "midi_ble_client.h"
#include "ui/menu.h"
#include "ui/menu_items.h"
#include "ui/stack.h"

void push_menu_bluetooth();

void disconnect() {
  midi_ble_client_disconnect();
  ui_clear();
}

void start_server() {
  if (midi_ble_client_is_initialized()){
    midi_ble_client_deinit();
  }
  midi_ble_server_init();
  ui_clear();
}

void stop() {
  if (midi_ble_server_is_initialized()){
    midi_ble_deinit();
  }
  if (midi_ble_client_is_initialized()){
    midi_ble_client_deinit();
  }
  ui_clear();
}

void start_client() {
  if (midi_ble_server_is_initialized()){
    midi_ble_deinit();
  }
  midi_ble_client_init();
  ui_clear();
  push_menu_bluetooth();
}

void push_menu_bluetooth() {
  ui_menu_t* menu = push_menu(4);
  if (midi_ble_client_is_initialized()) {
    if (midi_ble_client_is_connected()) {
      menu_add(menu, make_default_menu_item("Disconnect", disconnect));
    } else {
      menu_add(menu,
               make_default_menu_item("Connect", push_menu_bluetooth_connect));
    }
    menu_add(menu, make_default_menu_item("Remember connection",
                                          midi_ble_set_state_as_initial));
    menu_add(menu, make_default_menu_item("Switch to server", start_server));
    menu_add(menu, make_default_menu_item("Disable", stop));
    return;
  }

  if (midi_ble_server_is_initialized()) {
    menu_add(menu, make_default_menu_item("Switch to client", start_client));
    menu_add(menu, make_default_menu_item("Disable", stop));
    return;
  }

  menu_add(menu, make_default_menu_item("Start server", start_server));
  menu_add(menu, make_default_menu_item("Start client", start_client));
}
