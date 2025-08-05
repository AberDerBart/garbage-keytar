#include <stddef.h>

#include "./bluetooth_connect.h"
#include "midi_ble.h"
#include "midi_ble_client.h"
#include "ui/menu.h"
#include "ui_stack.h"

void start_server_nav(menu_item_t* _, ui_nav_t nav) { midi_ble_server_init(); }
menu_item_t mi_bluetooth_server_start = {
  label : "Start server",
  navigate : start_server_nav,
  free : NULL,
};

void stop_server_nav(menu_item_t* _, ui_nav_t nav) { midi_ble_deinit(); }
menu_item_t mi_stop_server = {
  label : "Disable",
  navigate : stop_server_nav,
  free : NULL,
};

void start_client_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    midi_ble_client_init();
  }
}
menu_item_t mi_bluetooth_start_client = {
  label : "Start client",
  navigate : start_client_nav,
  free : NULL,
};

void client_connect_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    push_menu_bluetooth_connect();
  }
}
menu_item_t mi_client_connect = {
  label : "Connect",
  navigate : client_connect_nav,
  free : NULL,
};

void client_disconnect_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    midi_ble_client_disconnect();
    ui_clear();
  }
}
menu_item_t mi_client_disconnect = {
  label : "Disconnect",
  navigate : client_disconnect_nav,
  free : NULL,
};

void client_remember_connection_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    midi_ble_set_state_as_initial();
  }
}
menu_item_t mi_client_remember_connection = {
  label : "Remeber connection",
  navigate : client_remember_connection_nav,
  free : NULL,
};

void stop_client_nav(menu_item_t* _, ui_nav_t nav) {
  if (nav == ENTER) {
    midi_ble_client_deinit();
  }
}
menu_item_t mi_stop_client = {
  label : "Disable",
  navigate : stop_client_nav,
  free : NULL,
};

menu_item_t* mi_bluetooth_children_off[] = {
    &mi_bluetooth_start_client,
    &mi_bluetooth_server_start,
    NULL,
};

menu_item_t* mi_bluetooth_children_client[] = {
    &mi_client_connect,
    &mi_client_remember_connection,
    &mi_stop_client,
    NULL,
};

menu_item_t* mi_bluetooth_children_server[] = {
    &mi_stop_server,
    NULL,
};

void push_menu_bluetooth() {
  if (midi_ble_client_is_initialized()) {
    if (midi_ble_client_is_connected()) {
      mi_bluetooth_children_client[0] = &mi_client_disconnect;
    } else {
      mi_bluetooth_children_client[0] = &mi_client_connect;
    }
    push_menu(mi_bluetooth_children_client);
    return;
  }

  if (midi_ble_server_is_initialized()) {
    push_menu(mi_bluetooth_children_server);
    return;
  }

  push_menu(mi_bluetooth_children_off);
}
