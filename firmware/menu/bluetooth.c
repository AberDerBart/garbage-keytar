#include <stddef.h>

#include "menu.h"
#include "midi_ble.h"
#include "midi_ble_client.h"

menu_item_t mi_bluetooth;

menu_item_t mi_bluetooth_server_start = {
  label : "Start server",
  parent : &mi_bluetooth,
  children : NULL,
  action : midi_ble_init,
};

menu_item_t mi_stop_server = {
  label : "Disable",
  parent : &mi_bluetooth,
  children : NULL,
  action : midi_ble_deinit,
};

menu_item_t mi_bluetooth_connect = {
  label : "Connect",
  parent : &mi_bluetooth,
  children : NULL,
  action : midi_ble_client_init,
};

menu_item_t mi_stop_client = {
  label : "Disable",
  parent : &mi_bluetooth,
  children : NULL,
  action : midi_ble_client_deinit,
};

menu_item_t* mi_bluetooth_children_off[] = {
    &mi_bluetooth_connect,
    &mi_bluetooth_server_start,
    NULL,
};

menu_item_t* mi_bluetooth_children_client[] = {
    &mi_stop_client,
    NULL,
};

menu_item_t* mi_bluetooth_children_server[] = {
    &mi_stop_server,
    NULL,
};

menu_item_t mi_bluetooth = {
  label : "Bluetooth",
  parent : &menu_item_main,
  children : mi_bluetooth_children_off,
  action : NULL,
};

void menu_update_bluetooth() {
  if (midi_ble_client_is_initialized()) {
    mi_bluetooth.children = mi_bluetooth_children_client;
  } else if (midi_ble_server_is_initialized()) {
    mi_bluetooth.children = mi_bluetooth_children_server;
  } else {
    mi_bluetooth.children = mi_bluetooth_children_off;
  }
}