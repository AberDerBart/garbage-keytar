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

menu_item_t mi_bluetooth_start_client = {
  label : "Start client",
  parent : &mi_bluetooth,
  children : NULL,
  action : midi_ble_client_init,
};

menu_item_t mi_bluetooth_devices[16];
menu_item_t* mi_bluetooth_device_list[17] = {NULL};

menu_item_t mi_client_connect = {
  label : "Connect",
  parent : &mi_bluetooth,
  children : mi_bluetooth_device_list,
  action : NULL,
};

menu_item_t mi_stop_client = {
  label : "Disable",
  parent : &mi_bluetooth,
  children : NULL,
  action : midi_ble_client_deinit,
};

menu_item_t* mi_bluetooth_children_off[] = {
    &mi_bluetooth_start_client,
    &mi_bluetooth_server_start,
    NULL,
};

menu_item_t* mi_bluetooth_children_client[] = {
    &mi_client_connect,
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

bool menu_update_bluetooth() {
  if (midi_ble_client_is_initialized()) {
    mi_bluetooth.children = mi_bluetooth_children_client;

    uint8_t device_count = midi_ble_client_device_count();
    for (int i = 0; i < device_count; i++) {
      mi_bluetooth_devices[i].action = NULL;
      mi_bluetooth_devices[i].children = NULL;
      mi_bluetooth_devices[i].label = midi_ble_client_get_device_name(i + 1);
      mi_bluetooth_devices[i].parent = &mi_client_connect;
      mi_bluetooth_device_list[i] = &mi_bluetooth_devices[i];
    }
    mi_bluetooth_device_list[device_count] = NULL;
  } else if (midi_ble_server_is_initialized()) {
    mi_bluetooth.children = mi_bluetooth_children_server;
  } else {
    mi_bluetooth.children = mi_bluetooth_children_off;
  }

  return true;
}